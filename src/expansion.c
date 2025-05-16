/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niclee <niclee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:13:03 by niclee            #+#    #+#             */
/*   Updated: 2025/05/16 11:43:04 by niclee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Fonctionnement:
    - On parcourt la liste des tokens.
    - Si le token est de type WORD et commence par '$', on appelle expand_env pour
      récupérer la valeur de l'environnement.
    - On remplace la valeur du token par la valeur récupérée.
    - Si la valeur est NULL, on ne fait rien.
    - On continue jusqu'à la fin de la liste.
Note:
    - La fonction expand_env doit être définie pour récupérer la valeur de l'environnement.
    - On suppose que les tokens sont déjà créés et que la liste est correctement formée.
    - On utilise free pour libérer la mémoire de l'ancienne valeur du token.
    - On utilise ft_strdup pour créer une nouvelle chaîne de caractères pour la valeur du token.
*/
void	expand_variables(t_token *tokens, char **env)
{
    t_token *current = tokens;
    char *expanded_value;

    while (current)
    {
        if (current->type == WORD)
        {
            // Ici, tu pourrais stocker l'info "quoted" dans le token si tu veux gérer les quotes simples
            expanded_value = expand_all_vars(current->value, env);
            if (expanded_value)
            {
                free(current->value);
                current->value = expanded_value;
            }
        }
        current = current->next;
    }
}

/*
Retourne:
    - La valeur de l'environnement correspondant à la variable d'environnement donnée.
    - NULL si la variable n'est pas trouvée.
Fonctionnement:
    - On parcourt la liste des variables d'environnement.
    - On compare la variable d'environnement avec la chaîne donnée.
    - Si on trouve une correspondance, on retourne la valeur de l'environnement.
    - Sinon, on continue jusqu'à la fin de la liste.
    - Si aucune correspondance n'est trouvée, on retourne NULL.
*/

char	*expand_env(char *str, char **env)
{
	int i;
	int len;

	if (!str || !env)
		return (NULL);
	len = ft_strlen(str);
	i = 0;
	while(env[i])
	{
		if (ft_strncmp(env[i], str, len) == 0 && env[i][len] == '=')
			return (ft_strdup(&env[i][len + 1]));
		i++;
	}
	return (NULL);
}

/*
Fonctionnement:
    - On parcourt la liste des tokens.
    - Si le token est de type WORD et contient un '*', on ouvre le répertoire courant.
    - On lit chaque entrée du répertoire et on compare avec le motif du token.
    - Si une correspondance est trouvée, on ajoute le nom de fichier à une nouvelle liste de tokens.
    - On remplace le token d'origine par la nouvelle liste de tokens.
    - On libère la mémoire de l'ancienne valeur du token.
    - On continue jusqu'à la fin de la liste.
Note:
    - On suppose que la fonction ft_match est définie pour comparer les chaînes avec le motif.
    - On utilise free pour libérer la mémoire de l'ancienne valeur du token.
    - On utilise ft_strdup pour créer une nouvelle chaîne de caractères pour la valeur du token.
*/

void	expand_wildcards(t_token *tokens)
{
    t_token	*current;
    DIR		*dir;
    struct dirent	*entry;
    t_token	*new_tokens;

    current = tokens;
    while (current)
    {
        if (current->type == WORD && ft_strchr(current->value, '*'))
        {
            dir = opendir(".");
            if (!dir)
                return ;
            new_tokens = NULL;
            while ((entry = readdir(dir)))
            {
                if (entry->d_name[0] != '.' && ft_match(entry->d_name, current->value))
                    add_token(&new_tokens, new_token(ft_strdup(entry->d_name), WORD));
            }
            closedir(dir);
            if (new_tokens)
            {
                current->type = new_tokens->type;
                free(current->value);
                current->value = ft_strdup(new_tokens->value);
                current->next = new_tokens->next;
                free(new_tokens->value);
                free(new_tokens);
            }
        }
        current = current->next;
    }
}

/*
Fonctionnement:
    - On compare la chaîne str avec le motif pattern.
    - Si le motif est vide, on retourne 1 si la chaîne est aussi vide.
    - Si le motif commence par '*', on vérifie si le reste du motif correspond à la chaîne
      ou si la chaîne correspond au reste du motif.
    - Si les caractères correspondent, on continue à comparer les caractères suivants.
    - Sinon, on retourne 0.
Note:
    - On utilise la récursion pour comparer les caractères.
    - On retourne 1 si une correspondance est trouvée, sinon 0.
*/

int	ft_match(const char *str, const char *pattern)
{
    if (*pattern == '\0')
        return (*str == '\0');
    if (*pattern == '*')
        return (ft_match(str, pattern + 1) || (*str && ft_match(str + 1, pattern)));
    if (*str == *pattern)
        return (ft_match(str + 1, pattern + 1));
    return (0);
}

/*
Objectif:
    - Prendre un string qui peut contenir des variable env (ex: "hello $USER $PATH")
    - Remplacer chaque variable $VAR par sa valeur trouvee dans env
    - Retourner la nouvelle chaine avec toutes les variable expansees

Fontionnement:
    - Cree une chaine vide qui va accumuler le resultat final
    - Parcour chaque caractere de la chaine d'entree avec un pointeur
    - Si le caractere courant est $ et que le suivant est unelettre, chiffre ou _
        - Avance le pointeur apres le $
        - Repere le debut du nom de la variable
        - Avance jusqu'a la fin du nom de la variable
        - Extrait le nom de la variable avec ft_substr
        - Cherche la valeur de la variable dans env
        - Si la variable existe, ajout dans result
    - Si ce n'est pas une variable, ajout simplement le caractere courant a result

Detail:
    - A chaque a jout a result, la memoire precedente est liberee pour eviter les fuites
    - Pas d'expansion pour $?
    - Ne gere pas les accolades (${VAR})
*/

char *expand_all_vars(const char *str, char **env)
{
    char *result = ft_calloc(1, 1);
    const char *p = str;
    const char *start;
    char *tmp;
    char *var;
    while (*p)
    {
        if (*p == '$' && (ft_isalnum(*(p+1)) || *(p+1) == '_'))
        {
            p++;
            start = p;
            while (ft_isalnum(*p) || *p == '_')
                p++;
            var = ft_substr(start, 0, p - start);
            char *val = expand_env(var, env);
            if (val)
            {
                tmp = ft_strjoin(result, val);
                free(result);
                result = tmp;
                free(val);
            }
            free(var);
        }
        else
        {
            char buf[2] = {*p, 0};
            tmp = ft_strjoin(result, buf);
            free(result);
            result = tmp;
            p++;
        }
    }
    return result;
}
