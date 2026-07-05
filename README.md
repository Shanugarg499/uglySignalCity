Core Architecture Summary: Real-Time Spatial Telemetry & RF Propagation Engine
This production-grade, multi-threaded C++ and MATLAB simulation engine models urban high-frequency radio frequency (RF) propagation and multi-wave phase interference across a high-density metropolitan landscape. Built on a modular 3-file C++ systems architecture, the project couples a high-performance native rendering pipeline (Raylib/rlgl) with a background engineering analytics data path (MATLAB R2026a Engine via native C++ API connection matrix).
System Architecture Pipeline
Plaintext
  [ USER SYSTEM INTERACTION ] ──> Precise 3D Ray-Casting & Coordinate Extraction
              │
              ▼
  [ MATHEMATICAL PHYSICS ENGINE ] ──> Dynamic Phase Multi-Wave Interferometry
              │                       Logarithmic Path Loss Solvers (Log-Distance Matrix)
              ▼
  [ GRAPHICS DISPLAY LAYER ] ──> Opaque Concrete Meshes & Inward-Facing Facades
                                  3D Topographic Amplitude Columns (Extruded Live Field)
Key Technical Implementations
3D Ray-Casting & Node Instantiation: Real-time translation of 2D screen coordinate inputs into 3D Cartesian coordinates (Y=0.5m asphalt base plane) using geometric viewport vectors. This allows interactive placement of unique hardware transmitters (Tx) and floating neon receiver (Rx) pylon structures.
Topographic Amplitude Extrusion Mesh: A high-speed spatial calculation grid that computes electromagnetic wave interference matrices. Instead of rendering a flat texture, the engine builds an extruded 3D column mesh where the Y-axis physically represents signal amplitude, mapping real-world constructive and destructive interference patterns in real time.
Deterministic Path Loss Telemetry Layer: Implements a localized log-distance path loss model:
Interactive 3D Ray-Casting Engine to map real-time viewport screen coordinates to precise physical spatial nodes, running complex logarithmic path loss solvers:
​(PL=20log 10(d)+32.4) over high-density mesh grids at stable 60 FPS.
This feeds a decoupled UI telemetry component that dynamically maps real-time signal degradation to standalone cell status indicators and color-shifting vectors (Green → Orange → Red).
Mac Hardware Optimization: Includes High-DPI resizing hints for Retina displays and handles explicit trackpad matrix transformations via low-level matrix isolation (rlPushMatrix / rlPopMatrix), preventing system gesture conflicts.
<img width="1358" height="701" alt="image" src="https://github.com/user-attachments/assets/cab98c9f-a5f2-4c07-8520-f109a89e94a5" />
