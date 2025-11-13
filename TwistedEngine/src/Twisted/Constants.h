#pragma once

namespace Twisted
{
	//project
	inline const std::filesystem::path PROJECT_FILE = "twisted.editor";
	inline const std::filesystem::path InfoFileExt = ".info";
	inline const std::filesystem::path InternalAssetExt = ".tasset";

	//world yaml keys
	inline const std::string ROOTS_SER_KEY = "roots";
	inline const std::string SYSTEMS_SER_KEY = "systems";
	inline const std::string UUID_SER_KEY = "uuid";
	inline const std::string NAME_SER_KEY = "name";
	inline const std::string COMPONENTS_SER_KEY = "components";
	inline const std::string TRANSFORM_SER_KEY = "transform";
	inline const std::string LIGHT_SER_KEY = "light";
	inline const std::string RENDERER_SER_KEY = "renderer";

	inline const std::string MODEL_MATRIX_NAME = "modelMat";
	inline const std::string VIEW_MATRIX_NAME = "viewMat";
	inline const std::string PROJ_MATRIX_NAME = "projMat";

}

