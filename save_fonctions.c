


// int add_redir_token(LEXER *input, int start, int end, t_token **list, int type) 
// {
//     t_token *new_node = create_new_token(input, start, end, type);
//     if (!new_node) return FALSE;
//     add_new_token(list, new_node);
//     return TRUE;
// }

// int add_filename_token(LEXER *input, int start, int end, t_token **list, int type) 
// {
//     t_token *new_node = create_new_token(input, start, end, type);
//     if (!new_node) return FALSE;
//     replace_prev_token(list, new_node);
//     return TRUE;
// }

// // int parse_filename(LEXER *input, t_token **list, int start)
// // {
// //     int end = input->head;
// //     if (!arg(input, list)) {
// //         input->head = start;
// //         return FALSE;
// //     }
// //     if ((int)input->head > end) 
// //         return add_filename_token(input, end, input->head, list, FILENAME);
// //     return TRUE;
// // }

// int parse_filename(LEXER *input, t_token **list, int start)
// {
//     int end = input->head;

//     ows(input); // Ignore les espaces
//     end = input->head;

//     if (!arg(input, list)) // Si aucun argument n'est trouvé, erreur
//     {
//         input->head = start;
//         return FALSE;
//     }

//     // Déterminer le type du token en fonction du contexte (DELIMITER pour HEREDOC)
//     int token_type = (list && (*list)->type == HEREDOC) ? DELIMITER : FILENAME;

//     return create_and_add_token(input, start, end, list, token_type);
// }

// int handle_redirect_out(LEXER *input, t_token **list)
// {
//     int start = input->head;
//     int end = input->head;

//     if (R_ARROW(input)) {
//         end = input->head; // fin après le premier '>'
        
//         int type = REDIRECT_OUT;
//         if (R_ARROW(input)) { // si un second '>' est trouvé
//             end = input->head; // fin après le second '>'
//             type = APPEND_OUT;
//         }

//         if (!add_redir_token(input, start, end, list, type))
//             return FALSE;

//         return parse_filename(input, list, input->head);
//     }
//     return FALSE;
// }

// int handle_redirect_in(LEXER *input, t_token **list)
// {
//     int start = input->head;
//     int end = input->head;

//     if (L_ARROW(input)) {
//         end = input->head; // fin après le premier '<'
        
//         int type = REDIRECT_IN;
//         if (L_ARROW(input)) { // si un second '<' est trouvé
//             end = input->head; // fin après le second '<'
//             type = HEREDOC;
//         }

//         if (!add_redir_token(input, start, end, list, type))
//             return FALSE;

//         return parse_filename(input, list, input->head);
//     }
//     return FALSE;
// }


int	redir(LEXER *input, t_token **list)
{
	int	start;
	int	end;
	t_token	*new_node;
	
	start = input->head;
	end = start; 
	if (R_ARROW(input))
	{
		end = input->head;
		if (R_ARROW(input))
		{
			end = input->head;
			if (end > start)
			{
				new_node = create_new_token(input, start, end, APPEND_OUT);
				add_new_token(list, new_node);
			}
			start = end;
			end = input->head;
			if (!arg(input, list))
			{
				input->head = start;
				return (FALSE);
			}
			start = end;
			end = input->head;
			if (end > start)
			{
				new_node = create_new_token(input, start, end, FILENAME);
				replace_prev_token(list, new_node);
			}
			return (TRUE);
		}
		// start = end;
		end = input->head;
		if (end > start)
		{
			new_node = create_new_token(input, start, end, REDIRECT_OUT);
			add_new_token(list, new_node);
		}
		ows(input);
		end = input->head;
		if (!arg(input, list))
		{
			input->head = start;
			return (FALSE);
		}
		start = end;
		end = input->head;
		if (end > start)
		{
			new_node = create_new_token(input, start, end, FILENAME);
			replace_prev_token(list, new_node);
		}
		return (TRUE);
	}
	else if (L_ARROW(input))
	{
		end =input->head;
		if (L_ARROW(input))
		{
			end = input->head;
			if (end > start)
			{
				new_node = create_new_token(input, start, end, HEREDOC);
				add_new_token(list, new_node);
			}
			ows(input);
			end = input->head;
			if (!arg(input, list))
			{
				input->head = start;
				return (FALSE);
			}
			start = end;
			end = input->head;
			if (end > start)
			{
				new_node = create_new_token(input, start, end, DELIMITER);
				replace_prev_token(list, new_node);
			}
			return (TRUE);
		}
		// start = end;
		end = input->head;
		if (end > start)
		{
			new_node = create_new_token(input, start, end, REDIRECT_IN);
			add_new_token(list, new_node);
		}
		ows(input);
		end = input->head;
		if (!arg(input, list))
		{
			input->head = start;
			return (FALSE);
		}
		start = end;
		end = input->head;
		if (end > start)
		{
			new_node = create_new_token(input, start, end, FILENAME);
			replace_prev_token(list, new_node);
		}
		return (TRUE);
	}
	input->head = start;
	return (FALSE);
}

int	command(LEXER *input, t_token **list)
{
	int	start;
	
	start = input->head;
	ows(input);
	if (!arg(input, list) && ows(input) && !redir(input, list))
	{
		input->head = start;
		return (FALSE);
	}
	ows(input);
	while (arg(input, list) || redir(input, list))
		ows(input); 
	return (TRUE);
}

/*
Sorry pour la treees longue fonction. C'est un travail pour notre 3e binome !
*/

// int	redir(LEXER *input, t_token **list)
// {
// 	int	start;
// 	int	end;
// 	t_token	*new_node;
	
// 	start = input->head;
// 	end = start; 
// 	if (R_ARROW(input))
// 	{
// 		end = input->head;
// 		if (R_ARROW(input))
// 		{
// 			end = input->head;
// 			if (end > start)
// 			{
// 				new_node = create_new_token(input, start, end, APPEND_OUT);
// 				add_new_token(list, new_node);
// 			}
// 			start = end;
// 			end = input->head;
// 			if (!arg(input, list))
// 			{
// 				input->head = start;
// 				return (FALSE);
// 			}
// 			start = end;
// 			end = input->head;
// 			if (end > start)
// 			{
// 				new_node = create_new_token(input, start, end, FILENAME);
// 				replace_prev_token(list, new_node);
// 			}
// 			return (TRUE);
// 		}
// 		// start = end;
// 		end = input->head;
// 		if (end > start)
// 		{
// 			new_node = create_new_token(input, start, end, REDIRECT_OUT);
// 			add_new_token(list, new_node);
// 		}
// 		ows(input);
// 		end = input->head;
// 		if (!arg(input, list))
// 		{
// 			input->head = start;
// 			return (FALSE);
// 		}
// 		start = end;
// 		end = input->head;
// 		if (end > start)
// 		{
// 			new_node = create_new_token(input, start, end, FILENAME);
// 			replace_prev_token(list, new_node);
// 		}
// 		return (TRUE);
// 	}
// 	else if (L_ARROW(input))
// 	{
// 		end =input->head;
// 		if (L_ARROW(input))
// 		{
// 			end = input->head;
// 			if (end > start)
// 			{
// 				new_node = create_new_token(input, start, end, HEREDOC);
// 				add_new_token(list, new_node);
// 			}
// 			ows(input);
// 			end = input->head;
// 			if (!arg(input, list))
// 			{
// 				input->head = start;
// 				return (FALSE);
// 			}
// 			start = end;
// 			end = input->head;
// 			if (end > start)
// 			{
// 				new_node = create_new_token(input, start, end, DELIMITER);
// 				replace_prev_token(list, new_node);
// 			}
// 			return (TRUE);
// 		}
// 		// start = end;
// 		end = input->head;
// 		if (end > start)
// 		{
// 			new_node = create_new_token(input, start, end, REDIRECT_IN);
// 			add_new_token(list, new_node);
// 		}
// 		ows(input);
// 		end = input->head;
// 		if (!arg(input, list))
// 		{
// 			input->head = start;
// 			return (FALSE);
// 		}
// 		start = end;
// 		end = input->head;
// 		if (end > start)
// 		{
// 			new_node = create_new_token(input, start, end, FILENAME);
// 			replace_prev_token(list, new_node);
// 		}
// 		return (TRUE);
// 	}
// 	input->head = start;
// 	return (FALSE);
// }

// int	command(LEXER *input, t_token **list)
// {
// 	int	start;
	
// 	start = input->head;
// 	ows(input);
// 	if (!arg(input, list) && ows(input) && !redir(input, list))
// 	{
// 		input->head = start;
// 		return (FALSE);
// 	}
// 	ows(input);
// 	while (arg(input, list) || redir(input, list))
// 		ows(input); 
// 	return (TRUE);
// }