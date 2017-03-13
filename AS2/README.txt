**************************************
* CMPUT 379 Assignment 2			       *
* By: Justin Widney and Oliver Rarog *
**************************************

Description:
The client can be executed in the following format: ./wbc379 'hostname' 'port number' [keyfile]
where hostname is the address of the server and port number is the port to which the 
server is bound to. The keyfile is an optional command if you have a keyfile and wish to 
encrypt/decrypt your information. The server can be executed by 
wbs379 portnumber {-f statefile | -n entries}. Where -f statefile will load the server
from a specified statefile and -n entries will load the server fresh with a max
of n entries.

The server should only exit upon receiving the SIGTERM signal at which point it will
close all connections and then dump the state of the whiteboard into a file called
'whiteboard.all'.

On the client side, the navigation and queries are done through the terminal.
Upon opening the client, the server will tell you how many entries are in
the whiteboard and you are given 2 options; option 1 interacts with the board
and option 2 exits the client program. There are 2 ways to interact with the whiteboard,
the first is viewing an entry which will ask you to select an entry (this value can never
exceed the intial size of the board given to you at the start of execution). This will return
the entry number and size as well as the information that is stored in the selected entry. The other
option is to update an entry which will ask you to select a entry, input some information that you
wish to store and finally if you want to encrypt this information. In order to encrypt and decrypt,
you must execute the client by providing a keyfile or else the client will fail to view and update
an entry that is encrypted. The return codes for the queries are as follows:
  -!XpY\nEntry\n = a response saying that entry X of length Y with information Entry
  -!XeY\nError\n = a status message for entry X with length Y with error message Error.
    (note: an error message of 0 length means success)

