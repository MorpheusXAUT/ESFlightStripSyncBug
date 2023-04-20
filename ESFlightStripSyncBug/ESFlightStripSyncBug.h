#pragma once

#include <string>
#include <sstream>
#include <windows.h>

#include "EuroScope/EuroScopePlugIn.h"

#include "constants.h"

namespace ESFlightStripSyncBug {
	class ESFlightStripSyncBug : public EuroScopePlugIn::CPlugIn {
	public:
		ESFlightStripSyncBug();
		virtual ~ESFlightStripSyncBug();

		void OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt, RECT Area);
		void OnFlightPlanFlightStripPushed(EuroScopePlugIn::CFlightPlan FlightPlan, const char* sSenderController, const char* sTargetController);

	private:
		void RegisterTagItems();

		void SetFlightStripAnnotation(const EuroScopePlugIn::CFlightPlan& fp, std::string msg, int index);

		void LogMessage(std::string message);
		void LogMessage(std::string message, std::string type);
	};
}