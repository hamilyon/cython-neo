; ModuleID = 'hello_world'
source_filename = "hello_world"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

@hello_str = private unnamed_addr constant [15 x i8] c"Hello, World!\0A\00", align 1

declare i32 @puts(ptr)

define i32 @main() {
entry:
  %0 = call i32 @puts(ptr @hello_str)
  ret i32 0
}
