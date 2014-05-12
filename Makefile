PARSER=ma-pa
SEARCHER=ma-se
CFLAGS=-I ./inc -L . 
CC=gcc $(CFLAGS)

all: $(PARSER) $(SEARCHER)
	chmod +x ./co-in
	ctags --langmap=c:.c.y -R ./*
	make -C ./httpd

mathtree.a: tree.o list.o mathtree.o
	ar rcs libmathtree.a $^

test-tree: test-tree.c mathtree.a 
	$(CC) $< -lmathtree -o $@

$(PARSER): $(PARSER).tab.o lex.yy.o mathtree.a
	$(CC) $(PARSER).tab.o lex.yy.o -lmathtree -lmcheck -lfl -o $@ 

$(SEARCHER): $(SEARCHER).c mathtree.a
	$(CC) $(word 1, $^) -lmathtree -o $@

%.tab.o: %.tab.c
	$(CC) -c $^ -o $@

lex.yy.o: lex.yy.c ma-pa.tab.h
	$(CC) -c  $< -include $(word 2, $^) -o $@

lex.yy.c: $(PARSER).l
	flex $<

%.o: %.c
	$(CC) $^ -c 

parse = bison --verbose --report=itemset -d $^
%.tab.h %.tab.c: %.y 
	$(parse) 2>&1 | grep --color conflicts || $(parse) 

clean:
	rm -f lex.yy.c *.output *.tab.h *.tab.c *.a *.o $(PARSER) test-tree query candy score tags rank $(SEARCHER)
	rm -rf collection
	make clean -C ./httpd
