CC = clang
CFLAGS = -fPIC

# Output executable name
EXEC = Main

# Object files list
OBJS = build/Main.o
OBJS += build/terminal/Terminal.o
OBJS += build/graphic/Color.o
OBJS += build/window/Window.o

# Build target
all: $(EXEC)

run: all
	#@changeToPublicPixelFont
	@clear
	@./Main
	@read -n 1 -s -r -p "Presiona cualquier tecla para continuar..."
	#@resetFont
	@clear

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(EXEC) build/*