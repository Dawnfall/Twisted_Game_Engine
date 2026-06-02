#pragma once

namespace Twisted
{
	//project
	inline constexpr const char* PROJECT_FILE = "twisted.editor";
	inline constexpr const char* InfoFileExt = ".info";
	inline constexpr const char* InternalAssetExt = ".tasset";

	//world yaml keys
	inline constexpr const char* ROOTS_SER_KEY = "roots";
	inline constexpr const char* SYSTEMS_SER_KEY = "systems";
	inline constexpr const char* COMPONENTS_SER_KEY = "components";
	inline constexpr const char* MANAGERS_SER_KEY = "managers";
	inline constexpr const char* UUID_SER_KEY = "uuid";
	inline constexpr const char* NAME_SER_KEY = "name";
	inline constexpr const char* TRANSFORM_SER_KEY = "transform";
	inline constexpr const char* LIGHT_SER_KEY = "light";
	inline constexpr const char* RENDERER_SER_KEY = "renderer";

	inline constexpr const char* MODEL_MATRIX_NAME = "modelMat";
	inline constexpr const char* VIEW_MATRIX_NAME = "viewMat";
	inline constexpr const char* PROJ_MATRIX_NAME = "projMat";

	//lights
	inline const char* DIR_LIGHT_NAME = "direct";
	inline const char* POINT_LIGHT_NAME = "point";
	inline const char* SPOT_LIGHT_NAME = "spot";

	inline const char* YAML_TYPE_FIELD = "type";
	inline const char* YAML_COLOR_FIELD = "color";
	inline const char* YAML_INTENSITY_FIELD = "intesity";
	inline const char* YAML_RANGE_FIELD = "range";
	inline const char* YAML_INNER_FIELD = "inner";
	inline const char* YAML_OUTER_FIELD = "outer";

}

