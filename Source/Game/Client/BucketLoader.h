#pragma once
#include <vector>
#include "../Shared/CubeDesc.h"

// this class does the saving and loading for buckets
class BucketLoader
{
public:
	static void saveBucket(const std::string& filename, const std::vector<CubeDesc>& cubes);
	
	BucketLoader(const std::string& filename);
	bool isOpen() const;
	const std::vector<CubeDesc>& getCubes() const;
private:
	static std::string colToString(uint32_t c);

};
