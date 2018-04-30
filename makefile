LEX = lex.yy.c
CFILE = main.c
LFILE = lexical.l
YFILE = syntax.y
SYNTAX = syntax.tab.c

parser: $(LEX) $(CFILE) $(SYNTAX) 
	gcc $(CFILE) $(SYNTAX) -lfl -ly -o parser
scanner:$(LEX) $(CFILE)
	cc $(CFILE) $(LEX) -lfl -o scanner
$(LEX): $(LFILE)
	flex $(LFILE)
$(SYNTAX): $(YFILE)
	bison -d $(YFILE)
	
.PHONY: clean test
test:
	./parser ../Test/test1.cmm
clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
