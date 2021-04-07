; ModuleID = 'Module'
source_filename = "Module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@0 = internal constant [3 x i8] c"%d\00"

declare i32 @printf(i8*, ...)

declare i32 @scanf(i8*, ...)

declare i32 @atoi(i8*)

declare double @atof(i8*)

declare i32 @sprintf(i8*, i8*, ...)

define i32 @getNumber(i32 %0) {
entry:
  %filler = alloca i32, align 4
  ret i32 5
}

define i32 @main() {
entry:
  %functionCall = call i32 @getNumber(i32 0)
  %functionCall1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i32 %functionCall)
  ret i32 0
}
