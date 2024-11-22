// CTRL -C in empty prompt
// display a new line with a new prompt
// echo $? must display 130

// ctrl-C after write some stuff

//     display a new line with a new prompt
//     echo $? must display 130

// ctrl-C in cat without arguments

//     ^C
//     echo $? must display 130


// ctrl-D on empty prompt

//     quit minishell
//     echo $? must display 0

// ctrl-D after write some stuff

//     do nothing

// ctrl-D in cat without arguments

//     close interactive mode
//     echo $? must display 0


// ctrl-\ on empty prompt

//     do nothing

// ctrl-\ after write some stuff

//     do nothing

// ctrl-\ in cat without arguments

//     \Quit (core dumped)
//     echo $? must display 131
