# Usage tutorial

For a most basic overview, you can always type `help` in the terminal, but not all commands are shown there due to memory restrictions.

# General terminal commands

`help` will display the help menu which will tell you about *most* commands.

`echo` will print anything you write behind it right back to the terminal - Usage input: `echo <text>`, output: `<text>`

`clear` will clear the terminal of any existing text on It, `clear` command doesn't take any arguments - Usage input: `clear`

`format` will format any string(s) you write behind It - Usage `format("<text", "<text>", "<text>")`, output: `<text> <text> <text>`

`loop` will loop any command behind It for the specified amount of times - Usage `loop <number> <command>`

`calc` will calculate any two numbers specified and with the operation you specified - Usage `calc <number> <operation> <number>`, output: `<result>`

 (And a secret command `cat`, which prints a cute little kitty to your terminal! :) prrr lol)

# File manipulation commands

`write` will write to a file - Usage input: `write <filename> <content>`, takes `\n` as sign to move to a new line, useful for scripts!

`read` will read from a file as long as It exists - Usage input: `read <filename>`, output: `<content>`

`ls` will list all existing files - Usage input: `ls`, output: `Files - ...`

`delete` will delete the specified file as long as It exists - Usage input `delete <filename>`

# Hardware commands

`high` will set the specified pin to high - Usage input: `high <pinnumber>`

`low` will set the specified pin to low - Usage input: `low <pinnumber>`

# G-Script run command

`run` will run any G-Script files specified - Usage input: `run <scriptname.gs>`

# G-Script usage

G-Script is very simple and is a way of automating commands within files. Essentially It just bundles commands and executes them in the order that they were typed in.

To let the system know that you are writing a G-Script file, It has to end with the .gs suffix. To run a G-Script file utilize the `run` command.







