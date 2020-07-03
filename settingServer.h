#include "httplib.h"
#include <string>
#pragma comment(lib, "ChilkatRel_x64.lib")
#pragma comment(lib, "ChilkatRelDll_x64.lib")

#include "CkJsonObject.h"
#include "CkRsa.h"
#include "CkHttp.h"
#include "CkGlobal.h"
#include "CkSettings.h"
#include "CkJsonObject.h"


namespace Settings {

    bool something1 = false;
    bool something2 = false;
    bool something_else = false;

}


namespace SettingServer {

    DWORD serverThread(PVOID) {
        printf("server thread started\n");
        bool unlocked = false;
        CkGlobal global;
        if (!unlocked) {
            if (!global.UnlockBundle("Unlicensed Chilkat Features")) {
                TerminateProcess(GetCurrentProcess(), 0);
            }
            unlocked = true;
        }
        CkSettings::initializeMultithreaded();
        using namespace httplib;
        using namespace Settings;

        Server svr;

        svr.Get("/telemetry", [](const Request& req, Response& res) {
            CkJsonObject json;
            json.UpdateBool("success", true);
            json.UpdateBool("settings.something1", something1);
            json.UpdateBool("settings.esp", something2);
            json.UpdateBool("settings.something_else", something_else);
            res.set_header("Access-Control-Allow-Origin", "*");

            res.set_content(json.emit(), "application/json");
        });

        svr.Get("/toggle", [](const Request& req, Response& res) {
            auto val = req.get_param_value("c");
            if (val == "something1") {
                Settings::something1 = !Settings::something1;
                printf("toggled something1\n");
            }
            else if (val == "something2") {
                Settings::something2 = !Settings::something2;
                printf("toggled something2\n");
            }
            else if (val == "something_else") {
                Settings::something_else = !Settings::something_else;
                printf("toggled something_else\n");

            }
            res.set_header("Access-Control-Allow-Origin", "*");

            res.set_content("{\"success\":\"true\"}", "application/json");
        });


        svr.set_error_handler([](const Request& req, Response& res) {
            CkJsonObject json;
            json.UpdateBool("success", false);
            json.UpdateInt("code", res.status);
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content(json.emit(), "application/json");
        });

        svr.Get("/", [](const Request& req, Response& res) {
            CkJsonObject json;
            json.UpdateBool("success", true);
            json.UpdateString("msg", "settings server");
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_content(json.emit(), "application/json");
        });

        svr.listen("127.0.0.1", 3005);
        return 0;
    }
}