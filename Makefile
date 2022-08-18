PROJECT_ROOT_PATH = `pwd`

VERSION     = 1.0.0

CC   =  gcc
DEBUG   =  -DUSE_DEBUG
CFLAGS  =  -Wall
SOURCES   =  $(wildcard *.c)
INCLUDES   =  -I
LIB_NAMES  =        # 比如我这里分别使用了静态库libfoo.a.1.0.0和libbar.so.1.0.0(默认已放到了/lib下)
LIB_PATH  =  -L./lib		  #使用到库的路径，一般放于/lib或在/usr/lib目录下 
OBJ   =  $(patsubst %.c, %.o, $(SOURCES)) #符合模式[%.c]的单词替换成[%.o] 即将.c文件转为.o文件 
TARGET  =  app

#links
$(TARGET):$(OBJ)
	$(CC) $(OBJ) $(LIB_PATH) $(LIB_NAMES) -o $(TARGET).$(VERSION)
	@rm -rf $(OBJ)
 
#compile
%.o: %.c
	$(CC) $(INCLUDES) $(DEBUG) -c $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	@echo "Remove linked and compiled files......"
	rm -rf $(OBJ) $(TARGET) output 