# ldd - Lumina Dummy Data Tool

Tool that allows exporting and importing dummy data into MELTY BLOOD: TYPE LUMINA. Currently supports update `2024/05/23`/`v1.46`; **not guaranteed** to work with future updates.

* Allows exporting the data from any of the five dummy slots into a readable text file.
* Allows importing the data from a saved file into any of the five slots of the game.
* Allows importing the data with a reversal timing, removing any empty frames before the first directional or button input.
* Allows manually clearing any of the slots as an added convenience through the user interface.

## Requirements

* VS C++ 2022 redistributable (https://aka.ms/vs/17/release/vc_redist.x86.exe)

## How to use

Opening `ldd.exe` will open the main window of the tool. All the buttons will be disabled initially if the game is not running yet. Launching Lumina, and then clicking the window, will enable all buttons except the Save buttons.
If Lumina was running before the tool was launched, it will behave as normal and enable all the Load, Load as Reversal, and Clear buttons.

Upon recording a dummy in any of the slots ingame, and then clicking on the tool's window, the length of the slot will be populated, as well as enable the corresponding Save button.


## Limitations and troubleshooting

* The tool does not extract save states.
* The tool does not handle the Rapid Beat Type for either player. This is important when sharing dummy data, as people might prefer to access Rapid Beat and Rapid Beat Launch differently, or not at all. (5AAA vs 5A 5A+B 5A+B, for instance)
* As the player, you must be on P1 side on character selection. The dummy data will only work properly if the dummy is on P2.

## Dummy data format

Upon exporting any dummy data, it will be extracted with the following format:

```
    frame: direction buttons
```

where `frame` can be either a single number or two numbers, separated by a dash (eg.: `22-25`);
`direction` is a number betweeen 0 and 9 (including 5); and `buttons` can be any combination of the characters A, B, C and D.

For instance, the tool will interpret

```
    51-70: 5ABCD
```

as holding from frame 51 to frame 70 the buttons A, B, C and D and the direction 5 (neutral).

When extracting, the tool won't register any frames where no directional or button input has been pressed, except for the very last frame, so the dummy data stays consistent in terms of length, while also saving some space.

When reading, the tool will interpret direction 0 and 5 as the same direction (neutral).

This should allow for a fine tuning of the dummy data.

Furthermore, you can insert comments with either `#` or `//` at the start of any line, or after any input.

```
    # this is a comment
    4: 5C // this is also another comment
```

Upon saving dummy data, it can be reloaded with reversal timing. This means that the tool will remove any frames that have no directional or button input until the very first frame that does have any meaningful input. This *should* be useful together with the Reversal Action, Guarding or Hit, and Same Time as Reversal Action Dummy Setting in-game.
Upon saving dummy data, it can be reloaded with reversal timing. This means that the tool will remove any frames that have no directional or button input until the very first frame that does have any meaningful input. This *should* be useful together with the Reversal Action, Guarding or Hit, and Same Time as Reversal Action Dummy Setting in-game.
