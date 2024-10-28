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