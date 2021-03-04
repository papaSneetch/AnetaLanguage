; ModuleID = 'Module'
source_filename = "Module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = internal constant [13 x i8] c"Hello World!\00"

declare i32 @puts(i8*)

define i32 @main() {
entry:
  %varPtr = alloca i8*, align 8
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i8** %varPtr, align 8
  %varPtr1 = load i8*, i8** %varPtr, align 8
  %functionCall = call i32 @puts(i8* %varPtr1)
  ret i32 0
}
