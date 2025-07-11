#include "ft_ping.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	pos;
	char	*string;
	int		totalsize;

	if ((!s1) || (!s2))
		return (NULL);
	totalsize = strlen(s1) + strlen(s2);
	pos = (size_t)totalsize + 1;
	string = (char *)malloc(sizeof(char) * (totalsize) + 1);
	if (!string)
		return (0);
	strlcpy(string, s1, strlen(s1) + 1);
	strlcat(string, s2, pos);
	string[strlen(string)] = 00;
	return (string);
}
