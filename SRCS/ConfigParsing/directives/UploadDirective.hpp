#ifndef UPLOADDIRECITVE_HPP
# define UPLOADDIRECITVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	UploadDirective : public ADirective
{
	public:
		UploadDirective(void);
		~UploadDirective(void);
	private:
		UploadDirective(const UploadDirective& other);
		UploadDirective&	operator=(const UploadDirective& other);

	public:
		void	parse(t_tokens& tokens, const std::string& config_path);
		void	setConfig(Config& config) const;
	
	private:
		bool	_on;
};

#endif
