; ModuleID = 'typeinfo.cpp'
source_filename = "typeinfo.cpp"
target datalayout = "e-m:x-p:32:32-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32-a:0:32-S32"
target triple = "i686-unknown-windows-msvc19.33.0"

module asm ".global \22??_7type_info@@6B@.1\22"
module asm "\22??_7type_info@@6B@.1\22 = \22??_7type_info@@6B@\22"

%union._SLIST_HEADER = type { i64 }
%rtti.TypeDescriptor15 = type { ptr, ptr, [16 x i8] }
%rtti.CompleteObjectLocator = type { i32, i32, i32, ptr, ptr }
%rtti.ClassHierarchyDescriptor = type { i32, i32, i32, ptr }
%rtti.BaseClassDescriptor = type { ptr, i32, i32, i32, i32, i32, ptr }

$"??_Gtype_info@@UAEPAXI@Z" = comdat any

$"??_7type_info@@6B@" = comdat largest

$"??_R0?AVtype_info@@@8" = comdat any

$"??_R4type_info@@6B@" = comdat any

$"??_R3type_info@@8" = comdat any

$"??_R2type_info@@8" = comdat any

$"??_R1A@?0A@EA@type_info@@8" = comdat any

@"?__type_info_root_node@@3T_SLIST_HEADER@@A" = dso_local global %union._SLIST_HEADER zeroinitializer, align 8
@__type_info_root_node_destructor_ptr = internal constant ptr @"?__type_info_root_node_destructor@@YAXXZ", section ".CRT$XTY", align 4
@0 = private unnamed_addr constant { [2 x ptr] } { [2 x ptr] [ptr @"??_R4type_info@@6B@", ptr @"??_Gtype_info@@UAEPAXI@Z"] }, comdat($"??_7type_info@@6B@")
@"??_7type_info@@6B@.1" = external constant ptr
@"??_R0?AVtype_info@@@8" = linkonce_odr global %rtti.TypeDescriptor15 { ptr @"??_7type_info@@6B@.1", ptr null, [16 x i8] c".?AVtype_info@@\00" }, comdat
@"??_R4type_info@@6B@" = linkonce_odr constant %rtti.CompleteObjectLocator { i32 0, i32 0, i32 0, ptr @"??_R0?AVtype_info@@@8", ptr @"??_R3type_info@@8" }, comdat
@"??_R3type_info@@8" = linkonce_odr constant %rtti.ClassHierarchyDescriptor { i32 0, i32 0, i32 1, ptr @"??_R2type_info@@8" }, comdat
@"??_R2type_info@@8" = linkonce_odr constant [2 x ptr] [ptr @"??_R1A@?0A@EA@type_info@@8", ptr null], comdat
@"??_R1A@?0A@EA@type_info@@8" = linkonce_odr constant %rtti.BaseClassDescriptor { ptr @"??_R0?AVtype_info@@@8", i32 0, i32 0, i32 -1, i32 0, i32 64, ptr @"??_R3type_info@@8" }, comdat
@llvm.used = appending global [1 x ptr] [ptr @__type_info_root_node_destructor_ptr], section "llvm.metadata"

@"??_7type_info@@6B@" = unnamed_addr alias ptr, getelementptr inbounds ({ [2 x ptr] }, ptr @0, i32 0, i32 0, i32 1)

; Function Attrs: mustprogress noinline optnone
define internal void @"?__type_info_root_node_destructor@@YAXXZ"() #0 {
  %1 = call i32 @__std_type_info_destroy_list(ptr noundef @"?__type_info_root_node@@3T_SLIST_HEADER@@A")
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone
define dso_local x86_thiscallcc void @"??1type_info@@UAE@XZ"(ptr noundef nonnull align 4 dereferenceable(12) %0) unnamed_addr #1 align 2 {
  %2 = alloca ptr, align 4
  store ptr %0, ptr %2, align 4
  %3 = load ptr, ptr %2, align 4
  store ptr @"??_7type_info@@6B@", ptr %3, align 4
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone
define linkonce_odr dso_local x86_thiscallcc noundef ptr @"??_Gtype_info@@UAEPAXI@Z"(ptr noundef nonnull align 4 dereferenceable(12) %0, i32 noundef %1) unnamed_addr #1 comdat align 2 {
  %3 = alloca ptr, align 4
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 4
  store i32 %1, ptr %4, align 4
  store ptr %0, ptr %5, align 4
  %6 = load ptr, ptr %5, align 4
  store ptr %6, ptr %3, align 4
  %7 = load i32, ptr %4, align 4
  call x86_thiscallcc void @"??1type_info@@UAE@XZ"(ptr noundef nonnull align 4 dereferenceable(12) %6) #4
  %8 = icmp eq i32 %7, 0
  br i1 %8, label %10, label %9

9:                                                ; preds = %2
  call void @"??3@YAXPAXI@Z"(ptr noundef %6, i32 noundef 12) #5
  br label %10

10:                                               ; preds = %9, %2
  %11 = load ptr, ptr %3, align 4
  ret ptr %11
}

declare dllimport i32 @__std_type_info_destroy_list(ptr noundef) #2

; Function Attrs: nobuiltin nounwind
declare dso_local void @"??3@YAXPAXI@Z"(ptr noundef, i32 noundef) #3

attributes #0 = { mustprogress noinline optnone "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="i686" "target-features"="+cmov,+cx8,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress noinline nounwind optnone "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="i686" "target-features"="+cmov,+cx8,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="i686" "target-features"="+cmov,+cx8,+x87" "tune-cpu"="generic" }
attributes #3 = { nobuiltin nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="i686" "target-features"="+cmov,+cx8,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind }
attributes #5 = { builtin nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.linker.options = !{!4, !5}
!llvm.ident = !{!6}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{i32 1, !"MaxTLSAlign", i32 65536}
!4 = !{!"/DEFAULTLIB:msvcrt.lib"}
!5 = !{!"/DEFAULTLIB:oldnames.lib"}
!6 = !{!"Debian clang version 19.1.7 (3+b1)"}
