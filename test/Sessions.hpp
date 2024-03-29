/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sessions.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbastian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 17:08:35 by lbastian          #+#    #+#             */
/*   Updated: 2024/01/26 19:03:22 by lbastian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SESSIONS_HPP
# define SESSIONS_HPP

#include <time.h>

#include <map>
#include <iostream>

class Sessions
{
	private:
		Sessions(const Sessions& sessions);
		Sessions& operator=(const Sessions& sessions);
		std::map<std::string, time_t>	_sessions_data;
		int 			check_date(int day, int month, int year);

	public:
		Sessions(void);
		~Sessions(void);
		std::string		create_token(void);
		void	add(std::string token, time_t date);
		bool	verify(std::string token);
		time_t	get_time_t(int sec, int min, int hour, int day, int month, int year);
		void	print_ses(void);
};


#endif
