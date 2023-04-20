#include "ESFlightStripSyncBug.h"

ESFlightStripSyncBug::ESFlightStripSyncBug::ESFlightStripSyncBug() :
	EuroScopePlugIn::CPlugIn(
		EuroScopePlugIn::COMPATIBILITY_CODE,
		PLUGIN_NAME,
		PLUGIN_VERSION,
		PLUGIN_AUTHOR,
		PLUGIN_LICENSE
	)
{
	std::ostringstream msg;
	msg << "Version " << PLUGIN_VERSION << " loaded.";

	this->LogMessage(msg.str());

	this->RegisterTagItems();
}

ESFlightStripSyncBug::ESFlightStripSyncBug::~ESFlightStripSyncBug()
{

}

void ESFlightStripSyncBug::ESFlightStripSyncBug::OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt, RECT Area)
{
	EuroScopePlugIn::CFlightPlan fp = this->FlightPlanSelectASEL();
	if (!fp.IsValid()) {
		return;
	}

	if (FunctionId == TAG_FUNC_SET_ALL_FLIGHT_STRIP_ANNOTATIONS) {
		for (int i = 0; i < 9; ++i) {
			std::ostringstream msg;
			msg << "all" << i;
			this->SetFlightStripAnnotation(fp, msg.str(), i);
		}
	}
	else if (FunctionId == TAG_FUNC_SET_WORKING_FLIGHT_STRIP_ANNOTATIONS) {
		for (int i = 0; i < 9; ++i) {
			if (i == 5) {
				continue;
			}
			std::ostringstream msg;
			msg << "working" << i;
			this->SetFlightStripAnnotation(fp, msg.str(), i);
		}
	} else if (FunctionId == TAG_FUNC_SET_BROKEN_FLIGHT_STRIP_ANNOTATION) {
		this->SetFlightStripAnnotation(fp, "broken5", 5);
	}
}

void ESFlightStripSyncBug::ESFlightStripSyncBug::OnFlightPlanFlightStripPushed(EuroScopePlugIn::CFlightPlan FlightPlan, const char* sSenderController, const char* sTargetController)
{
	if (!FlightPlan.IsValid()) {
		return;
	}

	auto cad = FlightPlan.GetControllerAssignedData();

	bool received = strcmp(sTargetController, this->ControllerMyself().GetCallsign()) == 0;
	for (int i = 0; i < 9; ++i) {
		std::ostringstream msg;
		msg << (received ? "Received" : "Pushed") << " flight strip annotation " << i << " has value: " << cad.GetFlightStripAnnotation(i);
		this->LogMessage(msg.str(), FlightPlan.GetCallsign());
	}
}

void ESFlightStripSyncBug::ESFlightStripSyncBug::RegisterTagItems()
{
	this->RegisterTagItemFunction("Set all flight strip annotations", TAG_FUNC_SET_ALL_FLIGHT_STRIP_ANNOTATIONS);
	this->RegisterTagItemFunction("Set working flight strip annotations", TAG_FUNC_SET_WORKING_FLIGHT_STRIP_ANNOTATIONS);
	this->RegisterTagItemFunction("Set broken flight strip annotation", TAG_FUNC_SET_BROKEN_FLIGHT_STRIP_ANNOTATION);
}

void ESFlightStripSyncBug::ESFlightStripSyncBug::SetFlightStripAnnotation(const EuroScopePlugIn::CFlightPlan& fp, std::string msg, int index)
{
	if (!fp.IsValid()) {
		return;
	}

	if (!fp.GetTrackingControllerIsMe() && strcmp(fp.GetTrackingControllerId(), "") != 0) {
		return;
	}

	auto cad = fp.GetControllerAssignedData();

	if (!cad.SetFlightStripAnnotation(index, msg.c_str())) {
		this->LogMessage("Failed to set message in flight strip annotations", fp.GetCallsign());
		return;
	}

	std::ostringstream oss;
	oss << "Successfully set flight strip annotation " << index << " to value: " << msg;
	this->LogMessage(oss.str(), fp.GetCallsign());
}

void ESFlightStripSyncBug::ESFlightStripSyncBug::LogMessage(std::string message)
{
	this->DisplayUserMessage("Message", PLUGIN_NAME, message.c_str(), true, true, true, false, false);
}

void ESFlightStripSyncBug::ESFlightStripSyncBug::LogMessage(std::string message, std::string type)
{
	this->DisplayUserMessage(PLUGIN_NAME, type.c_str(), message.c_str(), true, true, true, false, false);
}

ESFlightStripSyncBug::ESFlightStripSyncBug* pPlugin;

void __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn** ppPlugInInstance)
{
	*ppPlugInInstance = pPlugin = new ESFlightStripSyncBug::ESFlightStripSyncBug();
}

void __declspec (dllexport) EuroScopePlugInExit(void)
{
	delete pPlugin;
}