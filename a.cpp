#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

class SpaceObject {
protected:
    string id;
    double altitudeKm;
    double velocityKmS;

public:
    SpaceObject(const string& objId, double alt, double vel)
        : id(objId), altitudeKm(alt), velocityKmS(vel) {}

    virtual ~SpaceObject() = default;

    virtual void updateTrajectory(double atmosphericDensityFactor) = 0;

    virtual void displayStatus() const {
        cout << "[" << id << "] Alt: " << fixed << setprecision(2)
             << altitudeKm << " km | Vel: " << velocityKmS << " km/s";
    }

    bool hasDeorbited() const {
        return altitudeKm <= 100.0;
    }

    string getId() const { return id; }
    double getAltitude() const { return altitudeKm; }
    double getVelocity() const { return velocityKmS; }

    virtual string getType() const = 0;

    friend void AuditLog(const SpaceObject& obj);
};

class AtmosphericModel {
private:
    static double globalDragFactor;

public:
    static void setGlobalDragFactor(double factor) {
        if (factor > 0) {
            globalDragFactor = factor;
        }
    }

    static double getGlobalDragFactor() {
        return globalDragFactor;
    }
};

double AtmosphericModel::globalDragFactor = 1.0;

class RocketBodyDebris : public SpaceObject {
private:
    double dryMassKg;

public:
    RocketBodyDebris(const string& objId, double alt, double vel, double mass)
        : SpaceObject(objId, alt, vel), dryMassKg(mass) {}

    void updateTrajectory(double atmosphericDensityFactor) override {
        double decayRate = 0.05 * atmosphericDensityFactor * AtmosphericModel::getGlobalDragFactor();
        altitudeKm -= decayRate * (5000.0 / dryMassKg);
        velocityKmS -= decayRate * 0.01;
    }

    void displayStatus() const override {
        SpaceObject::displayStatus();
        cout << " | Type: Rocket Body (Mass: " << dryMassKg << " kg)\n";
    }

    string getType() const override { return "Rocket Body"; }
};

class DefunctSatellite : public SpaceObject {
private:
    bool solarPanelsDeployed;

public:
    DefunctSatellite(const string& objId, double alt, double vel, bool panels)
        : SpaceObject(objId, alt, vel), solarPanelsDeployed(panels) {}

    void updateTrajectory(double atmosphericDensityFactor) override {
        double areaMultiplier = solarPanelsDeployed ? 2.5 : 1.0;
        double decayRate = 0.03 * areaMultiplier * atmosphericDensityFactor * AtmosphericModel::getGlobalDragFactor();
        altitudeKm -= decayRate;
        velocityKmS -= decayRate * 0.005;
    }

    void displayStatus() const override {
        SpaceObject::displayStatus();
        cout << " | Type: Satellite [Panels: " 
             << (solarPanelsDeployed ? "Deployed" : "Retracted") << "]\n";
    }

    string getType() const override { return "Defunct Satellite"; }
};

void AuditLog(const SpaceObject& obj) {
    cout << "  [AUDIT] Raw Telemetry Readout -> ID: " << obj.id 
         << ", Alt: " << obj.altitudeKm << " km\n";
}

class OrbitalDecayTracker {
private:
    vector<unique_ptr<SpaceObject>> trackedDebris;
    static int totalObjectsTracked;
    int currentEpoch;

public:
    OrbitalDecayTracker() : currentEpoch(0) {}

    void addDebris(unique_ptr<SpaceObject> obj) {
        trackedDebris.push_back(move(obj));
        totalObjectsTracked++;
        cout << "-> Successfully added object to tracking radar.\n";
    }

    void checkCollisions(double thresholdKm = 5.0) {
        size_t n = trackedDebris.size();
        bool found = false;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                double distance = abs(trackedDebris[i]->getAltitude() - trackedDebris[j]->getAltitude());
                if (distance <= thresholdKm) {
                    cout << "  [WARNING: COLLISION RISK] " << trackedDebris[i]->getId()
                         << " and " << trackedDebris[j]->getId() 
                         << " are within " << fixed << setprecision(2)
                         << distance << " km altitude proximity!\n";
                    found = true;
                }
            }
        }
        if (!found) {
            cout << "  [INFO] No immediate collision risks detected.\n";
        }
    }

    void simulateNextEpoch(double atmosphericDensity) {
        if (trackedDebris.empty()) {
            cout << "-> No debris currently tracked. Please add objects first!\n";
            return;
        }

        currentEpoch++;
        cout << "\n=================================================================================\n";
        cout << " SIMULATING ORBITAL EPOCH " << currentEpoch 
             << " (Global Drag Multiplier: " << AtmosphericModel::getGlobalDragFactor() << ")\n";
        cout << "=================================================================================\n";
        
        for (auto& obj : trackedDebris) {
            obj->updateTrajectory(atmosphericDensity);
        }

        cout << "+-------------------+--------------------+--------------------+----------------------+\n";
        cout << "| Object ID         | Altitude (km)      | Velocity (km/s)    | Object Type          |\n";
        cout << "+-------------------+--------------------+--------------------+----------------------+\n";

        for (auto& obj : trackedDebris) {
            cout << "| " << left << setw(17) << obj->getId() 
                 << " | " << right << setw(18) << fixed << setprecision(2) << obj->getAltitude() 
                 << " | " << setw(18) << fixed << setprecision(2) << obj->getVelocity() 
                 << " | " << left << setw(20) << obj->getType() << " |\n";
        }
        
        cout << "+-------------------+--------------------+--------------------+----------------------+\n";

        for (auto& obj : trackedDebris) {
            if (obj->hasDeorbited()) {
                cout << "  [ALERT] Object " << obj->getId() << " has crossed Karman line (<=100km) and re-entered!\n";
            }
        }

        checkCollisions();
    }

    void runAudits() const {
        if (trackedDebris.empty()) {
            cout << "-> No objects to audit.\n";
            return;
        }
        cout << "\n--- Executing Safety Audit Log ---\n";
        for (const auto& obj : trackedDebris) {
            AuditLog(*obj);
        }
    }

    static int getTotalTrackedObjects() {
        return totalObjectsTracked;
    }
};

int OrbitalDecayTracker::totalObjectsTracked = 0;


int main() {
    OrbitalDecayTracker tracker;
    int choice = 0;

    while (true) {
        cout << "\n====================================\n";
        cout << " SPACE DEBRIS TRACKER MENU\n";
        cout << "====================================\n";
        cout << "1. Add Rocket Body Debris\n";
        cout << "2. Add Defunct Satellite Debris\n";
        cout << "3. Simulate Next Epoch (Update Orbit)\n";
        cout << "4. Change Global Drag Multiplier\n";
        cout << "5. Run System Audit Log\n";
        cout << "6. Exit Program\n";
        cout << "Enter choice (1-6): ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            string id;
            double alt, vel, mass;
            cout << "Enter Object ID (e.g. RB-2024): ";
            cin >> id;
            cout << "Enter Altitude in km (e.g. 350.0): ";
            cin >> alt;
            cout << "Enter Velocity in km/s (e.g. 7.6): ";
            cin >> vel;
            cout << "Enter Dry Mass in kg (e.g. 1500): ";
            cin >> mass;

            tracker.addDebris(make_unique<RocketBodyDebris>(id, alt, vel, mass));

        } else if (choice == 2) {
            string id;
            double alt, vel;
            int panelChoice;
            cout << "Enter Object ID (e.g. SAT-101): ";
            cin >> id;
            cout << "Enter Altitude in km (e.g. 300.0): ";
            cin >> alt;
            cout << "Enter Velocity in km/s (e.g. 7.5): ";
            cin >> vel;
            cout << "Are Solar Panels Deployed? (1 for Yes, 0 for No): ";
            cin >> panelChoice;

            tracker.addDebris(make_unique<DefunctSatellite>(id, alt, vel, panelChoice == 1));

        } else if (choice == 3) {
            double density;
            cout << "Enter Atmospheric Density Factor (Standard is 1.0): ";
            cin >> density;
            tracker.simulateNextEpoch(density);

        } else if (choice == 4) {
            double newFactor;
            cout << "Current Drag Factor: " << AtmosphericModel::getGlobalDragFactor() << "\n";
            cout << "Enter New Global Drag Factor (e.g., 2.5 for Solar Flare): ";
            cin >> newFactor;
            AtmosphericModel::setGlobalDragFactor(newFactor);
            cout << "-> Global Drag Factor updated successfully.\n";

        } else if (choice == 5) {
            tracker.runAudits();

        } else if (choice == 6) {
            cout << "Exiting Space Debris Decay Tracker. Goodbye!\n";
            break;

        } else {
            cout << "Invalid menu choice. Try again.\n";
        }
    }

    return 0;
}