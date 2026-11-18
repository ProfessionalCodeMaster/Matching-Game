# Matching Game

A small Windows matching-game board built with C++ and SFML. The board shuffles two copies each of A, B, C, and D. Left-click a card to reveal it; click it again to hide it.

## Build and run

1. Install Visual Studio with the C++ desktop development workload and [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-msbuild).
2. Install SFML 3.1 for x64 with `vcpkg install sfml:x64-windows` and run `vcpkg integrate install` to integrate vcpkg with Visual Studio.
3. Open `Matching Game.slnx`, select the **x64** platform, and build and run the project.

The game uses the Windows Segoe UI font from the Windows Fonts directory. The SFML runtime DLLs must be available alongside the executable when distributing it; the vcpkg Visual Studio integration copies them to the build output.
