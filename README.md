# Online-Voting-System

This project is a console-based online voting system implemented in C. It allows voters and candidates to register, authenticate, and participate in an election securely. The system manages multiple voters and candidates, ensures one vote per person, and displays the election results with winner, runner-up, and second runner-up.


Features

Voter Registration – Users can register with name, email, and password.

Candidate Registration – Candidates can register with their details.

Login System – Secure login with email and password.

Voting – Authenticated voters can cast their vote (only once).

Election Management – Admin-like functionality to end the election.

Result Declaration – Displays Winner, Runner-up, and Second Runner-up.

Menu-driven UI – Interactive console menu for smooth navigation.


Tech Stack

Language: C

Libraries Used: <stdio.h>, <string.h>, <stdlib.h>, <stdbool.h>


How to Run

Clone this repository:

    git clone https://github.com/your-username/online-voting-system.git


Navigate to the project folder:

    cd online-voting-system


Compile the code:

    gcc logic.c -o voting


Run the executable:

./voting
