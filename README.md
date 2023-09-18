# RTCW-CheatLib
Cheat Library for Return to Castle Wolfenstein
This DLL library provides functionality to modify the game Return to Castle Wolfenstein. Development was based on source code analysis and reverse engineering of the game's libraries.


## Cheat Features:
- Increases the length of the ~~dick~~ knife by modifying the respective constant.
- Provides invulnerability from both enemy shots and falls.
- Speed hack: slightly increased base speed, with an additional boost during sprinting.


## File Descriptions:

- **Injector.cs** 
  - C# source for the DLL injector targeting WolfSP, with a hard-coded DLL name.

- **RTCW-Cheat.js**
  - Frida script used for function insights and diverse tests.

- **dllmain.cpp**
  - The primary cheat component's source, embodying the cheat's functionality in a DLL.

## Usage:
- Place both the DLL and injector in the same folder.
- Launch the WolfSP game.
- Run the injector.
- The injected DLL will establish the required hooks and modify data once the `qagamex86.dll` library, containing the logic we need, appears.

## Known Issues:
- The cheat may become non-functional if you load a saved game during gameplay without going back to the main menu or dying (which you won't with the cheat). To resolve this, return to the main menu and load your saved game from there.
---

**Disclaimer:** This cheat library is provided for educational purposes only. The author is not responsible for any misuse or potential damage caused by this software. Use at your own risk.


## Contact & Support:

For updates and more info, join my Telegram channel: [CheatCrafting101](https://t.me/cheatdevelop)
