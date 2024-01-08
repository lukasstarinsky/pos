CLIENT_INC=-IClient/ThirdParty/Ignis/Engine/src/Ignis \
		   -IClient/ThirdParty/Ignis/Engine/ThirdParty/imgui \
		   -IClient/ThirdParty/Ignis/Engine/ThirdParty/glm \
		   -IClient/ThirdParty/Ignis/Engine/ThirdParty/glad/include
CLIENT_SRC=Client/src/Main.cpp \
		   Client/src/Pong.cpp \
		   Client/src/Cube.cpp \
		   Client/src/MessageQueue.cpp \
		   Client/src/UDPSocket.cpp
CLIENT_LIBS=-lIgnisEngine -limgui -ldwmapi -lws2_32 -lglad -lGdi32 -lopengl32 -lstdc++ -LBuild
CLIENT_OBJS=$(patsubst Client/src/%.cpp,Build/Client/%.o,$(CLIENT_SRC))

Build/Client/%.o: Client/src/%.cpp
	gcc -std=c++23 $(CLIENT_INC) -c $< -o $@ $(CLIENT_LIBS)

Client: $(CLIENT_OBJS)
	gcc -std=c++23 $(CLIENT_INC) $^ -o ./Build/$@ $(CLIENT_LIBS)

# Server... 
SERVER_SRC=Server/src/Main.cpp \
		   Server/src/Game.cpp \
		   Server/src/UDPSocket.cpp
SERVER_OBJS=$(patsubst Server/src/%.cpp,Build/Server/%.o,$(SERVER_SRC))

Build/Server/%.o: Server/src/%.cpp
	gcc -std=c++23 -c $< -o $@

Server: $(SERVER_OBJS)
	gcc -std=c++23 $^ -o ./Build/$@

Clean:
ifeq ($(OS), Windows_NT)
	del /q .\Build\Client\*
	del /q .\Build\Server\*
else
	rm -rf ./Build/Client/*
	rm -rf ./Build/Server/*
endif

All: Client Server