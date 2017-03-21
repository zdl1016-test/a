TARGET = test
TARGET2 = api
TARGET3 = test_markupstl
all:$(TARGET) $(TARGET2) $(TARGET3)


$(TARGET):test.cpp Markup.cpp
	g++ -Wall -Werror $^ -o $@
	
$(TARGET2):api.cpp Markup.cpp
	g++ -Wall -Werror $^ -o $@

$(TARGET3):test_markupstd.cpp markupstl.cpp markup.cpp
	g++ -Wall -Werror $^ -o $@
