Project is a bootable shell program that reads files into memory and executes programs.

All Steps Are Completed

To verify the code
  ./compileOS
  java -jar simulator.jar

And use the disk.img as disk c
In the booted terminal ("C>")
Running "exec tstpr2" should output
  tstpr2 is working!
Running "type messag" should output
	"If this message prints out, then your readSector function is working correctly"

For error testing
Running "exec tstpr3" should output
  File not found!
Running "type tstpr3" should output
  File not found!

https://github.com/AmbivalentLeather/COMP350-ProjectC


