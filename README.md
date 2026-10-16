# 🛰️ Space Debris & Orbital Decay Tracker

An Object-Oriented C++ simulation that models Low Earth Orbit (LEO) space debris trajectories, atmospheric drag decay, Karman line atmospheric re-entry, and orbital proximity collision risks.

---

## 📌 Project Overview

With thousands of defunct satellites and spent rocket stages orbiting the Earth, space debris poses an ever-increasing risk to spaceflight safety and orbital operations. 

This project simulates orbital decay across discrete time epochs by modeling atmospheric drag on various types of space debris, dynamically detecting collision hazards, and alerting operators when objects de-orbit through the Karman line ($\le 100\text{ km}$).

---

## 🧱 Object-Oriented Programming (OOP) Architecture

The project is structured according to core Object-Oriented principles in Modern C++:

### 1. **Inheritance & Class Hierarchy**
- `SpaceObject`: Abstract base class representing any tracked orbital object with common attributes such as `id`, `altitudeKm`, and `velocityKmS`.
- `RocketBodyDebris`: Inherits from `SpaceObject`. Models heavy, spent rocket stages where decay rate is inversely proportional to dry mass.
- `DefunctSatellite`: Inherits from `SpaceObject`. Models dead satellites with deployable solar panels, where surface area drag multiplier directly influences orbital decay.

### 2. **Polymorphism & Abstraction**
- Pure virtual functions `updateTrajectory(double atmosphericDensityFactor)` and `getType()` enforce a consistent interface across debris types.
- Runtime dynamic dispatch via `virtual void displayStatus() const` and a `virtual ~SpaceObject() = default;` ensuring safe polymorphic cleanup.

### 3. **Encapsulation & Data Hiding**
- Member variables are strictly protected or private, accessed and modified through controlled public member methods and getters (`getId()`, `getAltitude()`, `getVelocity()`).

### 4. **Static Members & Global Environmental State**
- `AtmosphericModel`: Encapsulates a static drag factor (`globalDragFactor`) simulating solar storms or atmospheric variations affecting all orbital bodies simultaneously.
- `OrbitalDecayTracker`: Maintains `totalObjectsTracked` static counter across tracker lifecycles.

### 5. **Friend Function**
- `AuditLog(const SpaceObject& obj)`: Declared as a `friend` of `SpaceObject` to provide low-level telemetry logging by directly inspecting internal object state for safety checks without exposing internal variables to the public interface.

### 6. **Modern C++ Memory Management**
- Employs `std::unique_ptr<SpaceObject>` and `std::vector` in `OrbitalDecayTracker` to manage polymorphic debris instances safely with automatic lifetime management and zero raw-pointer memory leaks.

---

## 🚀 Features

- 🛸 **Debris Registry**: Add rocket bodies (with customizable dry mass) and satellites (with configurable solar array deployment).
- 🌌 **Orbital Decay Simulation**: Step through epochs with customizable atmospheric density factors.
- 💥 **Collision Proximity Detection**: Pairwise altitude proximity scanning to alert when two objects are within collision danger thresholds ($< 5\text{ km}$).
- 🔥 **Karman Line Alert**: Notifies when debris crosses below $100\text{ km}$ and burns up in the atmosphere.
- ☀️ **Solar Weather Modeling**: Dynamically adjust the global atmospheric drag factor (e.g., simulating a solar storm expansion of the upper atmosphere).
- 📋 **Safety Telemetry Audit**: Friend function audit logs for raw telemetry readouts.

---

## 🛠️ Building and Running

### Prerequisites
- Any C++14 (or later) compliant compiler (`g++`, `clang++`, or `MSVC`)

### Compilation

Using **g++ (MinGW / GCC)**:
```bash
g++ -std=c++14 -O2 a.cpp -o space_debris_tracker
```

Using **Clang**:
```bash
clang++ -std=c++14 -O2 a.cpp -o space_debris_tracker
```

Using **MSVC (Developer Command Prompt)**:
```cmd
cl /EHsc /std:c++14 a.cpp /Fe:space_debris_tracker.exe
```

### Running

**Windows (PowerShell / Command Prompt)**:
```powershell
.\space_debris_tracker.exe
```

**Linux / macOS**:
```bash
./space_debris_tracker
```

---

## 💻 Sample Program Walkthrough

```text
====================================
 SPACE DEBRIS TRACKER MENU
====================================
1. Add Rocket Body Debris
2. Add Defunct Satellite Debris
3. Simulate Next Epoch (Update Orbit)
4. Change Global Drag Multiplier
5. Run System Audit Log
6. Exit Program
Enter choice (1-6): 1
Enter Object ID (e.g. RB-2024): RB-Falcon
Enter Altitude in km (e.g. 350.0): 340
Enter Velocity in km/s (e.g. 7.6): 7.68
Enter Dry Mass in kg (e.g. 1500): 4000
-> Successfully added object to tracking radar.
```

---

## 📄 License

This project is created for educational and academic coursework purposes. Free to use and reference.
