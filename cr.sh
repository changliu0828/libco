find $1 -name "*.c" -o -name "*.cpp" -o -name "*.java" -o -name "*.h" -o -name "*.swift" -o -name "*.php" -o -name "*.m" | xargs awk '{a++;}; /(.+)/{l++;}; /\*\//{i=i+NR;}; /\/\*/{n=n+NR; m++}; /\/\//{x++}; END {print "/***************Result***************/"; print "Code Annotation Rate: "(i-n+m+x)/l; print "Code Lines without blank lines: "l; print "Code Annotation Lines NO.: "i-n+m+x; print "All Lines NO.: "a}'