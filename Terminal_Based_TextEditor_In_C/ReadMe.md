# Text Editor and Spell Check

Our Text Editor Uses Stacks to Impliment the Undo and Redo Functions. <br />
There is a Seperate stack created for each of these operations. <br />
It also Uses a Linked Queue to Impliment the Dictionary that we use for the Spell-Checker.

The Text Editor is Menu Based and at the Beginneng needs the Path of the File that needs to be Edited as the Input. If we wish to create a new File then at the start of the Program, Enter "N" as the input and it creates a new file in the present working directory.

### Operations Supported
* Append
* Undo & Redo
* Rename, Delete
* Search (Not Case Sensitive)
* Find and Replace (Case Sensitive)

The Spell Checker uses a Dictionary File as a Database for validating the words in the given file and Displays all the Misspelled words, This is not case sensitive. 

* To Run the Editor, Compile Text_Editor.c using GCC
* The Sample.txt File is an Extract from a story and hence, Has no spelling errors.
* The Other File has errors.
* The Spell-Checker will take sometime (around 5-7 seconds) as the Dictionary file has 3,70,107 words.



