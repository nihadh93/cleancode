#include <iostream>
#include <fstream>
#include <string>
#include <vector>
 
using namespace std;
 
// Base interface for all monitors (optional for scalability)
class IMonitor {
public:
    virtual void check() = 0;
    virtual vector<string> getAlerts() const = 0;
    virtual void clearAlerts() = 0;
    virtual ~IMonitor() = default;
};
 
class TemperatureMonitor : public IMonitor {
private:
    float temperature = 0;
    vector<string> alerts;
public:
    void update(float value) {
        temperature = value;
    }
 
    void check() override {
        alerts.clear();
        if (temperature > 35.0) {
            alerts.push_back("ALERT: High temperature detected (Environment)");
        }
    }
 
    vector<string> getAlerts() const override {
        return alerts;
    }
 
    void clearAlerts() override {
        alerts.clear();
    }
};
 
class DimensionMonitor : public IMonitor {
private:
    float variation = 0;
    vector<string> alerts;
public:
    void update(float value) {
        variation = value;
    }
 
    void check() override {
        alerts.clear();
        if (variation > 0.05) {
            alerts.push_back("ALERT: Part-dimension variation too high (Machine)");
        }
    }
 
    vector<string> getAlerts() const override {
        return alerts;
    }
 
    void clearAlerts() override {
        alerts.clear();
    }
};
 
class OperationMonitor : public IMonitor {
private:
    int minutes = 0;
    vector<string> alerts;
public:
    void update(int value) {
        minutes = value;
    }
 
    void check() override {
        alerts.clear();
        if (minutes > 360) {
            alerts.push_back("ALERT: Continuous operation exceeded 6 hours (Machine)");
        }
    }
 
    vector<string> getAlerts() const override {
        return alerts;
    }
 
    void clearAlerts() override {
        alerts.clear();
    }
};
 
class SelfTestMonitor : public IMonitor {
private:
    int code = 0;
    vector<string> alerts;
public:
    void update(int value) {
        code = value;
    }
 
    void check() override {
        alerts.clear();
        switch (code) {
            case 0xFF:
                alerts.push_back("Self-test OK");
                break;
            case 0x00:
                alerts.push_back("ALERT: No data from machine (Environment)");
                break;
            case 0x01:
                alerts.push_back("ALERT: Controller board failure (Machine)");
                break;
            case 0x02:
                alerts.push_back("ALERT: Configuration data corrupted (Machine)");
                break;
            default:
                alerts.push_back("ALERT: Unknown self-test code (Machine)");
        }
    }
 
    vector<string> getAlerts() const override {
        return alerts;
    }
 
    void clearAlerts() override {
        alerts.clear();
    }
};
 
class CNCMachineMonitor {
private:
    TemperatureMonitor tempMonitor;
    DimensionMonitor dimMonitor;
    OperationMonitor opMonitor;
    SelfTestMonitor selfTestMonitor;
 
public:
    void processInput(float temp, float variation, int minutes, int selfTestCode) {
        tempMonitor.update(temp);
        dimMonitor.update(variation);
        opMonitor.update(minutes);
        selfTestMonitor.update(selfTestCode);
 
        tempMonitor.check();
        dimMonitor.check();
        opMonitor.check();
        selfTestMonitor.check();
 
        printAlerts(tempMonitor);
        printAlerts(dimMonitor);
        printAlerts(opMonitor);
        printAlerts(selfTestMonitor);
 
        cout << "---" << endl;
 
        tempMonitor.clearAlerts();
        dimMonitor.clearAlerts();
        opMonitor.clearAlerts();
        selfTestMonitor.clearAlerts();
    }
 
private:
    void printAlerts(const IMonitor& monitor) {
        for (const auto& alert : monitor.getAlerts()) {
            cout << alert << endl;
        }
    }
};
 
int main() {
    CNCMachineMonitor monitor;
    ifstream infile("cnc_data.txt");
 
    if (!infile) {
        cerr << "Error opening input file." << endl;
        return 1;
    }
 
    float temp, variation;
    int minutes, selfTest;
 
    while (infile >> temp >> variation >> minutes >> hex >> selfTest) {
        monitor.processInput(temp, variation, minutes, selfTest);
    }
 
    infile.close();
    return 0;
}
