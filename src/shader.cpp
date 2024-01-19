#include "shader.hpp"

namespace herbglitch {
    Shader::Shader(): id(0), uniformModel(0), uniformProjection(0), pointLightCount(0), spotLightCount(0){}
    Shader::~Shader(){ clear(); }

    void Shader::createFromString(const char* vertexCode, const char* fragmentCode){
        compile(vertexCode, fragmentCode);
    }

    void Shader::createFromFiles(const char* vertexPath, const char* fragmentPath){
        std::string vertexString = readFile(vertexPath);
        std::string fragmentString = readFile(fragmentPath);

        const char* vertexCode = vertexString.c_str();
        const char* fragmentCode = fragmentString.c_str();

        compile(vertexCode, fragmentCode);
    }

    std::string Shader::readFile(const char* path){
        std::string content;
        std::ifstream fileStream(path, std::ios::in);

        if(!fileStream.is_open()){
            printf("failed to read '%s'. File does not exist", path);
            return "";
        }

        std::string line = "";
        while(!fileStream.eof()){
            std::getline(fileStream, line);
            content.append(line + "\n");
        }

        fileStream.close();
        return content;
    }

    void Shader::use(){ glUseProgram(id); }

    void Shader::clear(){
        if(id != 0){
            glDeleteProgram(id);
            id = 0;
        }

        uniformModel = 0;
        uniformProjection = 0;
    }

    void Shader::compile(const char* vertexCode, const char* fragmentCode){
        id = glCreateProgram();

        if(!id){
            printf("Error creating shader program!\n");
            return;
        }

        add(id, vertexCode, GL_VERTEX_SHADER);
        add(id, fragmentCode, GL_FRAGMENT_SHADER);

        GLint result = 0;
        GLchar eLog[1024] = { 0 };

        glLinkProgram(id);
        glGetProgramiv(id, GL_LINK_STATUS, &result);
        if(!result){
            glGetProgramInfoLog(id, sizeof(eLog), NULL, eLog);
            printf("Error linking program: '%s'\n", eLog);
            return;
        }

        glValidateProgram(id);
        glGetProgramiv(id, GL_VALIDATE_STATUS, &result);
        if(!result){
            glGetProgramInfoLog(id, sizeof(eLog), NULL, eLog);
            printf("Error validating program: '%s'\n", eLog);
            return;
        }

        uniformModel = glGetUniformLocation(id, "model");
        uniformProjection = glGetUniformLocation(id, "projection");
        uniformView = glGetUniformLocation(id, "view");
        uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(id, "directionalLight.base.ambientIntensity");
        uniformDirectionalLight.uniformColor = glGetUniformLocation(id, "directionalLight.base.color");
        uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(id, "directionalLight.base.diffuseIntensity");
        uniformDirectionalLight.uniformDirection = glGetUniformLocation(id, "directionalLight.direction");
        uniformSpecularIntensity = glGetUniformLocation(id, "material.specularIntensity");
        uniformShininess = glGetUniformLocation(id, "material.shininess");
        uniformEyePosition = glGetUniformLocation(id, "eyePosition");

        uniformPointLightCount = glGetUniformLocation(id, "pointLightCount");
        for(size_t i = 0; i < MAX_POINT_LIGHTS; i++){
            char locBuff[100] = {'\0'};

            snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
            uniformPointLight[i].uniformColor = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
            uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
            uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
            uniformPointLight[i].uniformPosition = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
            uniformPointLight[i].uniformConstant = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
            uniformPointLight[i].uniformLinear = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
            uniformPointLight[i].uniformExponent = glGetUniformLocation(id, locBuff);
        }

        uniformSpotLightCount = glGetUniformLocation(id, "spotLightCount");
        for(size_t i = 0; i < MAX_SPOT_LIGHTS; i++){
            char locBuff[100] = {'\0'};

            snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
            uniformSpotLight[i].uniformColor = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
            uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
            uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
            uniformSpotLight[i].uniformPosition = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
            uniformSpotLight[i].uniformConstant = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
            uniformSpotLight[i].uniformLinear = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
            uniformSpotLight[i].uniformExponent = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
            uniformSpotLight[i].uniformDirection = glGetUniformLocation(id, locBuff);

            snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
            uniformSpotLight[i].uniformEdge = glGetUniformLocation(id, locBuff);
        }
    }

    void Shader::add(GLuint theProgram, const char* shaderCode, GLenum shaderType){
        GLuint theShader = glCreateShader(shaderType);

        const GLchar* theCode[1];
        theCode[0] = shaderCode;

        GLint codeLength[1];
        codeLength[0] = strlen(shaderCode);

        glShaderSource(theShader, 1, theCode, codeLength);
        glCompileShader(theShader);

        GLint result = 0;
        GLchar eLog[1024] = {0};

        glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
        if (!result) {
            glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
            printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
            return;
        }

        glAttachShader(theProgram, theShader);
    }

    void Shader::setDirectionalLight(light::Directional *dLight){
        dLight->use(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor, uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
    }

    void Shader::setPointLights(light::Point *pLight, unsigned int lightCount){
        if(lightCount > MAX_POINT_LIGHTS){ lightCount = MAX_POINT_LIGHTS; }

        glUniform1i(uniformPointLightCount, lightCount);

        for(size_t i = 0; i < lightCount; i++){
            pLight[i].use(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor, uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition, 
                          uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
        }
    }

    void Shader::setSpotLights(light::Spot *sLight, unsigned int lightCount){
        if(lightCount > MAX_SPOT_LIGHTS){ lightCount = MAX_POINT_LIGHTS; }

        glUniform1i(uniformSpotLightCount, lightCount);

        for(size_t i = 0; i < lightCount; i++){
            sLight[i].use(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor, uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition,  uniformSpotLight[i].uniformDirection,
                          uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent, uniformSpotLight[i].uniformEdge);
        }
    }
}