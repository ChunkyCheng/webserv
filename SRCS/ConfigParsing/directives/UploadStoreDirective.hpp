#ifndef UPLOADSTOREDIRECTIVE_HPP
# define UPLOADSTOREDIRECTIVE_HPP

# include "ConfigParser.h"
# include "ADirective.hpp"

class	UploadStoreDirective : public ADirective
{
	public:
		UploadStoreDirective(void);
		~UploadStoreDirective(void);
	private:
		UploadStoreDirective(const UploadStoreDirective& other);
		UploadStoreDirective&	operator=(const UploadStoreDirective& other);

	public:
		void	setConfig(Config& config) const;
};

#endif
