/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   port.h                                                                   */
/*                                                                            */
/*   By: Tiago <tiagoalvarezschiaffino@gmail.com>                             */
/*                                                             / \__          */
/*                                                            (    @\___      */
/*                                                             /         O    */
/*   Created: 2024/05/16 04:51:08 by Tiago                    /   (_____/     */
/*   Updated: 2024/05/16 05:59:26 by Tiago                  /_____/ U         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PORT_H
# define PORT_H

# define PORT			8080
# define BUFFER_SIZE	30000

char	*ft_itoa(int n)
{
	size_t		i;
	long int	an;
	char		*output;

	an = n;
	i = (n <= 0);
	while (n != 0 && ++i > 0)
		n = n / 10;
	output = malloc(sizeof(char) * (i + 1));
	if (output == NULL)
		return (NULL);
	if (an < 0)
		output[0] = '-';
	if (an < 0)
		an *= -1;
	output[i--] = '\0';
	if (an == 0)
		output[0] = 48;
	while (an)
	{
		output[i--] = (an % 10) + '0';
		an /= 10;
	}
	return (output);
}

int	ft_getwc(char const *s, char c)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] == '\0')
			break ;
		while (s[i] != '\0' && s[i] != c)
			i++;
		count++;
	}
	return (count);
}

char	*ft_strndup(const char *str, size_t n)
{
	size_t	i;
	char	*output;

	output = malloc(sizeof(char) * n + 1);
	if (output == NULL)
		return (NULL);
	i = -1;
	while (++i < n)
		output[i] = str[i];
	output[i] = '\0';
	return (output);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	count;
	char	*wordstart;
	char	**output;

	if (!s)
		return (NULL);
	count = ft_getwc(s, c);
	output = malloc(sizeof(char *) * (count + 1));
	if (output == NULL)
		return (NULL);
	i = 0;
	while (*s != '\0')
	{
		while (*s == c)
			s++;
		if (*s == '\0')
			break ;
		wordstart = (char *)s;
		while (*s != '\0' && *s != c)
			s++;
		output[i++] = ft_strndup(wordstart, s - wordstart);
	}
	output[i] = 0;
	return (output);
}

#endif