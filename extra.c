
int		is_dummy(char const *name)
{
	return (name[0] == '.' && (!name[1] || (name[1] == '.' && !name[2])));
}

