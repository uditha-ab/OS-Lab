Compiling & Running

To compile the c source file and run the program use following commands in commandline.


    gcc my_shell.c -o my_shell
    ./my_shell
  
When "$" sign shows up you can enter the commands.
Following commands are supported,
pwd,
ls,
sleep,
cat,
ps

Only Part A of the lab is implemented so ctrl+C will close the terminal
Hope to implement that soon.

Can write the commands to a .txt file and run them all as a batch using following command

  
  ./my_shell command.txt
