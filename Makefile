CC=g++

SRC_SVM=svm_run.cpp svm.cpp svm_parser.cpp
SRC_IMP=imp_compiler.cpp imp.cpp imp_parser.cpp imp_printer.cpp imp_typechecker.cpp imp_type.cpp imp_interpreter.cpp imp_codegen.cpp

svm: $(SRC_SVM)
	$(CC) -o svm $(SRC_SVM)

compiler: $(SRC_IMP)
	$(CC) -o compile  $(SRC_IMP)

