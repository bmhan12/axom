!
! Copyright (c) 2015, Lawrence Livermore National Security, LLC.
! Produced at the Lawrence Livermore National Laboratory.
!
! All rights reserved.
!
! This source code cannot be distributed without permission and
! further review from Lawrence Livermore National Laboratory.
!

module sidre_view
  use iso_c_binding
  use fruit
  use sidre_mod
  implicit none

contains
!------------------------------------------------------------------------------

  subroutine create_views()
    type(datastore) ds
    type(datagroup) root
    type(dataview) dv_0, dv_1
    type(databuffer) db_0, db_1

    call set_case_name("create_views")

    ds = datastore_new()
    root = ds%get_root()

    dv_0 = root%create_view_and_allocate("field0", SIDRE_INT_ID, 1)
    dv_1 = root%create_view_and_allocate("field1", SIDRE_INT_ID, 1)

    db_0 = dv_0%get_buffer()
    db_1 = dv_1%get_buffer()

    call assert_equals(db_0%get_index(), 0, "db_0%get_index(), 0")
    call assert_equals(db_1%get_index(), 1, "db_1%get_index(), 1")
    call ds%delete()
  end subroutine create_views

!------------------------------------------------------------------------------

  subroutine int_buffer_from_view()
    type(datastore) ds
    type(datagroup) root
    type(dataview) dv
    integer(C_INT), pointer :: data(:)
    integer i

    call set_case_name("int_buffer_from_view")

    ds = datastore_new()
    root = ds%get_root()

    dv = root%create_view_and_allocate("u0", SIDRE_INT_ID, 10_8)
    call assert_equals(dv%get_type_id(), SIDRE_INT_ID, "dv%get_type_id(), SIDRE_INT_ID")
    call dv%get_data(data)

    do i = 1, 10
       data(i) = i * i
    enddo

    call dv%print()

!--    !  EXPECT_EQ(ATK_dataview_get_total_bytes(dv), dv->getSchema().total_bytes())
!--    call assert_equals(dv%get_total_bytes(), sizeof(int) * 10)
    call ds%delete()
  end subroutine int_buffer_from_view

!------------------------------------------------------------------------------

  subroutine int_buffer_from_view_conduit_value()
    type(datastore) ds
    type(datagroup) root
    type(dataview) dv
    integer(C_INT), pointer :: data(:)
    integer i

    call set_case_name("int_buffer_from_view_conduit")

    ds = datastore_new()
    root = ds%get_root()

    dv = root%create_view_and_allocate("u0", SIDRE_INT_ID, 10_8)
    call dv%get_data(data)

    do i = 1, 10
       data(i) = i * i
    enddo

    call dv%print()

!--    EXPECT_EQ(ATK_dataview_get_total_bytes(dv), sizeof(int) * 10)
    call ds%delete()
  end subroutine int_buffer_from_view_conduit_value

!------------------------------------------------------------------------------

  subroutine int_array_multi_view()
    type(datastore) ds
    type(datagroup) root
    type(databuffer) dbuff
    type(dataview) dv_e, dv_o 
    type(C_PTR) data_ptr
    integer(C_INT), pointer :: data(:)
    integer i

    call set_case_name("int_array_multi_view")

    ds = datastore_new()
    root = ds%get_root()
    dbuff = ds%create_buffer(SIDRE_INT_ID, 10_8)

    call dbuff%allocate()
    data_ptr = dbuff%get_void_ptr()
    call c_f_pointer(data_ptr, data, [ 10 ])

    do i = 1, 10
       data(i) = i
    enddo

    call dbuff%print()

!--#ifdef XXX
!--    EXPECT_EQ(dbuff->getNode().schema().total_bytes(),
!--              dbuff->getSchema().total_bytes())
!--#endif

    dv_e = root%create_view("even", dbuff)
    dv_o = root%create_view("odd", dbuff)
    call assert_true(dbuff%get_num_views() == 2, "dbuff%get_num_views() == 2")

!--#ifdef XXX
!--  dv_e->apply(DataType::uint32(5,0,8))
!--
!--  dv_o->apply(DataType::uint32(5,4,8))
!--
!--  call dv_e%print()
!--  call dv_o%print()
!--
!--  uint32_array dv_e_ptr = dv_e->getNode().as_uint32_array()
!--  uint32_array dv_o_ptr = dv_o->getNode().as_uint32_array()
!--  for(int i=0  i<5  i++)
!--  {
!--    std::cout << "idx:" <<  i
!--              << " e:" << dv_e_ptr[i]
!--              << " o:" << dv_o_ptr[i]
!--              << " em:" << dv_e_ptr[i]  % 2
!--              << " om:" << dv_o_ptr[i]  % 2
!--              << std::endl
!--
!--    EXPECT_EQ(dv_e_ptr[i] % 2, 0u)
!--    EXPECT_EQ(dv_o_ptr[i] % 2, 1u)
!--  }
!--#endif
    call ds%print()
    call ds%delete()
  end subroutine int_array_multi_view

!------------------------------------------------------------------------------

  subroutine init_int_array_multi_view()
    type(datastore) ds
    type(datagroup) root
    type(databuffer) dbuff
    type(dataview) dv_e, dv_o 
    type(C_PTR) data_ptr
    integer, pointer :: data(:)
    integer i
    
    call set_case_name("init_int_array_multi_view")

    ds = datastore_new()
    root = ds%get_root()
    dbuff = ds%create_buffer()
    
    call dbuff%allocate(SIDRE_INT_ID, 10_8)
    data_ptr = dbuff%get_void_ptr()
    call c_f_pointer(data_ptr, data, [ 10 ])

    do i = 1, 10
       data(i) = i
    enddo

    call dbuff%print()

!--#ifdef XXX
!--  EXPECT_EQ(dbuff->getNode().schema().total_bytes(),
!--            dbuff->getSchema().total_bytes())
!--#endif

    dv_e = root%create_view("even", dbuff)
    dv_o = root%create_view("odd", dbuff)

!--#ifdef XXX
!--  ! uint32(num_elems, offset, stride)
!--  dv_e->apply(DataType::uint32(5,0,8))
!--
!--
!--  ! uint32(num_elems, offset, stride)
!--  dv_o->apply(DataType::uint32(5,4,8))
!--
!--
!--  call dv_e%print()
!--  call dv_o%print()
!--
!--  uint32_array dv_e_ptr = dv_e->getNode().as_uint32_array()
!--  uint32_array dv_o_ptr = dv_o->getNode().as_uint32_array()
!--  for(int i=0  i<5  i++)
!--  {
!--    std::cout << "idx:" <<  i
!--              << " e:" << dv_e_ptr[i]
!--              << " o:" << dv_o_ptr[i]
!--              << " em:" << dv_e_ptr[i]  % 2
!--              << " om:" << dv_o_ptr[i]  % 2
!--              << std::endl
!--
!--    EXPECT_EQ(dv_e_ptr[i] % 2, 0u)
!--    EXPECT_EQ(dv_o_ptr[i] % 2, 1u)
!--  }
!--#endif

    call ds%print()
    call ds%delete()
  end subroutine init_int_array_multi_view

!------------------------------------------------------------------------------

  subroutine int_array_depth_view()
    type(datastore) ds
    type(datagroup) root
    type(databuffer) dbuff
    type(dataview) view0
    type(dataview) view1
    type(dataview) view2
    type(dataview) view3
    integer(C_INT), pointer :: data(:)
    type(C_PTR) data_ptr
    integer i
    integer(C_LONG) depth_nelems
    integer(C_LONG) total_nelems

    call set_case_name("int_array_depth_view")

    ! create our main data store
    ds = datastore_new()

    depth_nelems = 10 
    total_nelems = 4 * depth_nelems

    dbuff = ds%create_buffer(SIDRE_INT_ID, total_nelems)

    ! get access to our root data Group
    root = ds%get_root()

    ! Allocate buffer to hold data for 4 "depth" views
    call dbuff%allocate()

    data_ptr = dbuff%get_void_ptr()
    call c_f_pointer(data_ptr, data, [ total_nelems ])

    do i = 1, total_nelems
       data(i) = (i - 1) / depth_nelems
    enddo

    call dbuff%print()

    call assert_true( dbuff%get_num_elements() == 4 * depth_nelems, "dbuff%get_num_elements() == 4 * depth_nelems" )

    ! create 4 "depth" views and apply offsets into buffer
    view0 = root%create_view_into_buffer("view0", dbuff)
    call view0%apply_nelems_offset(depth_nelems, 0 * depth_nelems)

    view1 = root%create_view_into_buffer("view1", dbuff)
    call view1%apply_nelems_offset(depth_nelems, 1 * depth_nelems)

    view2 = root%create_view_into_buffer("view2", dbuff)
    call view2%apply_nelems_offset(depth_nelems, 2 * depth_nelems)

    view3 = root%create_view_into_buffer("view3", dbuff)
    call view3%apply_nelems_offset(depth_nelems, 3 * depth_nelems)

    call assert_true( dbuff%get_num_views() == 4, "dbuff%get_num_views() == 4" )

    call view0%print()
    call view1%print()
    call view2%print()
    call view3%print()

    ! check values in depth views...
    call view0%get_data(data)
    call assert_true(all(data == 0), "depth 0 does not compare")

    call view1%get_data(data)
    call assert_true(all(data == 1), "depth 1 does not compare")
 
    call view2%get_data(data)
    call assert_true(all(data == 2), "depth 2 does not compare")

    call view3%get_data(data)
    call assert_true(all(data == 3), "depth 3 does not compare")

    call ds%print()
    call ds%delete()

  end subroutine int_array_depth_view

!------------------------------------------------------------------------------

  subroutine int_array_view_attach_buffer()
    type(datastore) ds
    type(datagroup) root
    type(databuffer) dbuff
    type(dataview) field0
    type(dataview) field1
    integer(C_INT), pointer :: data(:)
    type(C_PTR) data_ptr
    integer i
    integer(C_LONG) field_nelems
    integer(C_LONG) elem_count

    call set_case_name("int_array_view_attach_buffer")

    ! create our main data store
    ds = datastore_new()

    ! get access to our root data Group
    root = ds%get_root()

    field_nelems = 10 

    ! create 2 "field" views with type and # elems
    elem_count = 0
    field0 = root%create_view("field0", SIDRE_INT_ID, field_nelems)
    elem_count = elem_count + field0%get_num_elements()
    print*,"elem_count field0",elem_count
    field1 = root%create_view("field1", SIDRE_INT_ID, field_nelems)
    elem_count = elem_count + field1%get_num_elements()
    print*,"elem_count field1",elem_count

    call assert_true( elem_count == 2 * field_nelems, "elem_count == 2 * field_nelems" )

    ! Create buffer to hold data for all fields and allocate
    dbuff = ds%create_buffer(SIDRE_INT_ID, elem_count)
    call dbuff%allocate()

    call assert_true( dbuff%get_num_elements() == elem_count, "dbuff%get_num_elements() == elem_count" ) 

    ! Initilize buffer data for testing below
    data_ptr = dbuff%get_void_ptr()
    call c_f_pointer(data_ptr, data, [ elem_count ])

    do i = 1, elem_count
       data(i) = (i - 1) / field_nelems
    enddo

    call dbuff%print()

    ! attach field views to buffer and apply offsets into buffer
    call field0%attach_buffer(dbuff)
    call field0%apply_nelems_offset(field_nelems, 0 * field_nelems);
    call field1%attach_buffer(dbuff)
    call field1%apply_nelems_offset(field_nelems, 1 * field_nelems);

    call assert_true( dbuff%get_num_views() == 2, "dbuff%get_num_views() == 2" )

    ! print field views...
    call field0%print()
    call field1%print()

    ! check values in field views...
    call field0%get_data(data)
    call assert_true(size(data) == field_nelems, &
         "depth 0 is incorrect size")
    call assert_true(all(data == 0), "depth 0 does not compare")

    call field1%get_data(data)
    call assert_true(size(data) == field_nelems, &
         "depth 1 is incorrect size")
    call assert_true(all(data == 1), "depth 0 does not compare")

    call ds%print()
    call ds%delete()

  end subroutine int_array_view_attach_buffer

!------------------------------------------------------------------------------

  subroutine int_array_multi_view_resize()
     !
     ! This example creates a 4 * 10 buffer of ints,
     ! and 4 views that point the 4 sections of 10 ints
     !
     ! We then create a new buffer to support 4*12 ints
     ! and 4 views that point into them
     !
     ! after this we use the old buffers to copy the values
     ! into the new views
     !
    type(datastore) ds
    type(datagroup) root, r_old
    type(dataview) base_old
    integer(C_INT), pointer :: data(:)
    integer i

    call set_case_name("int_array_multi_view_resize")

    ! create our main data store
    ds = datastore_new()

    ! get access to our root data Group
    root = ds%get_root()

    ! create a group to hold the "old" or data we want to copy
    r_old = root%create_group("r_old")

    ! create a view to hold the base buffer and allocate
    ! we will create 4 sub views of this array
    base_old = r_old%create_view_and_allocate("base_data", SIDRE_INT_ID, 40)

    call base_old%get_data(data)

    ! init the buff with values that align with the
    ! 4 subsections.
    data( 1:10) = 1
    data(11:20) = 2
    data(21:30) = 3
    data(31:40) = 4

!--#ifdef XXX
!--  ! setup our 4 views
!--  ATK_databuffer * buff_old = ATK_dataview_get_buffer(base_old)
!--  call buff_old%print()
!--  ATK_dataview * r0_old = ATK_dataview_create_view(r_old, "r0",buff_old)
!--  ATK_dataview * r1_old = ATK_dataview_create_view(r_old, "r1",buff_old)
!--  ATK_dataview * r2_old = ATK_dataview_create_view(r_old, "r2",buff_old)
!--  ATK_dataview * r3_old = ATK_dataview_create_view(r_old, "r3",buff_old)
!--
!--  ! each view is offset by 10 * the # of bytes in a uint32
!--  ! uint32(num_elems, offset)
!--  index_t offset =0
!--  r0_old->apply(r0_old, DataType::uint32(10,offset))
!--
!--  offset += sizeof(int) * 10
!--  r1_old->apply(r1_old, DataType::uint32(10,offset))
!--
!--  offset += sizeof(int) * 10
!--  r2_old->apply(r2_old, DataType::uint32(10,offset))
!--
!--  offset += sizeof(int) * 10
!--  r3_old->apply(r3_old, DataType::uint32(10,offset))
!--
!--  ! check that our views actually point to the expected data
!--  !
!--  uint32 * r0_ptr = r0_old->getNode().as_uint32_ptr()
!--  for(int i=0  i<10  i++)
!--  {
!--    EXPECT_EQ(r0_ptr[i], 1u)
!--    ! check pointer relation
!--    EXPECT_EQ(&r0_ptr[i], &data_ptr[i])
!--  }
!--
!--  uint32 * r3_ptr = r3_old->getNode().as_uint32_ptr()
!--  for(int i=0  i<10  i++)
!--  {
!--    EXPECT_EQ(r3_ptr[i], 4u)
!--    ! check pointer relation
!--    EXPECT_EQ(&r3_ptr[i], &data_ptr[i+30])
!--  }
!--
!--  ! create a group to hold the "old" or data we want to copy into
!--  ATK_datagroup * r_new = ATK_datagroup_create_group(root, "r_new")
!--  ! create a view to hold the base buffer
!--  ATK_dataview * base_new = ATK_datagroup_create_view_and_buffer(r_new, "base_data")
!--
!--  ! alloc our buffer
!--  ! create a buffer to hold larger subarrays
!--  base_new->allocate(base_new, DataType::uint32(4 * 12))
!--  int* base_new_data = (int *) ATK_databuffer_det_data(base_new)
!--  for (int i = 0 i < 4 * 12 ++i) 
!--  {
!--     base_new_data[i] = 0
!--  } 
!--
!--  ATK_databuffer * buff_new = ATK_dataview_get_buffer(base_new)
!--  call buff_new%print()
!--
!--  ! create the 4 sub views of this array
!--  ATK_dataview * r0_new = ATK_datagroup_create_view(r_new, "r0",buff_new)
!--  ATK_dataview * r1_new = ATK_datagroup_create_view(r_new, "r1",buff_new)
!--  ATK_dataview * r2_new = ATK_datagroup_create_view(r_new, "r2",buff_new)
!--  ATK_dataview * r3_new = ATK_datagroup_create_view(r_new, "r3",buff_new)
!--
!--  ! apply views to r0,r1,r2,r3
!--  ! each view is offset by 12 * the # of bytes in a uint32
!--
!--  ! uint32(num_elems, offset)
!--  offset =0
!--  r0_new->apply(DataType::uint32(12,offset))
!--
!--  offset += sizeof(int) * 12
!--  r1_new->apply(DataType::uint32(12,offset))
!--
!--  offset += sizeof(int) * 12
!--  r2_new->apply(DataType::uint32(12,offset))
!--
!--  offset += sizeof(int) * 12
!--  r3_new->apply(DataType::uint32(12,offset))
!--
!--  ! update r2 as an example first
!--  call buff_new%print()
!--  call r2_new%print()
!--
!--  ! copy the subset of value
!--  r2_new->getNode().update(r2_old->getNode())
!--  call r2_new%print()
!--  call buff_new%print()
!--
!--
!--  ! check pointer values
!--  int * r2_new_ptr = (int *) ATK_dataview_get_data_pointer(r2_new)
!--
!--  for(int i=0  i<10  i++)
!--  {
!--    EXPECT_EQ(r2_new_ptr[i], 3)
!--  }
!--
!--  for(int i=10  i<12  i++)
!--  {
!--    EXPECT_EQ(r2_new_ptr[i], 0)     ! assumes zero-ed alloc
!--  }
!--
!--
!--  ! update the other views
!--  r0_new->getNode().update(r0_old->getNode())
!--  r1_new->getNode().update(r1_old->getNode())
!--  r3_new->getNode().update(r3_old->getNode())
!--
!--  call buff_new%print()
!--#endif

    call ds%print()
    call ds%delete()

  end subroutine int_array_multi_view_resize

!------------------------------------------------------------------------------

  subroutine int_array_realloc()
    !
    ! info
    !
    type(datastore) ds
    type(datagroup) root
    type(dataview) a1, a2
!    type(C_PTR) a1_ptr, a2_ptr
    real(C_FLOAT), pointer :: a1_data(:)
    integer(C_INT), pointer :: a2_data(:)
    integer i

    call set_case_name("int_array_realloc")

    ! create our main data store
    ds = datastore_new()

    ! get access to our root data Group
    root = ds%get_root()

    ! create a view to hold the base buffer
    a1 = root%create_view_and_allocate("a1", SIDRE_FLOAT_ID, 5)
    a2 = root%create_view_and_allocate("a2", SIDRE_FLOAT_ID, 5)

    call a1%get_data(a1_data)
    call a2%get_data(a2_data)

    call assert_true(size(a1_data) == 5, &
         "a1_data is incorrect size")
    call assert_true(size(a2_data) == 5, &
         "a2_data is incorrect size")

!--  EXPECT_EQ(ATK_dataview_get_total_bytes(a1), sizeof(float)*5)
!--  EXPECT_EQ(ATK_dataview_get_total_bytes(a2), sizeof(int)*5)

    a1_data(1:5) =  5.0
    a2_data(1:5) = -5

    call a1%reallocate(10)
    call a2%reallocate(15)

    call a1%get_data(a1_data)
    call a2%get_data(a2_data)

    call assert_true(size(a1_data) == 10, &
         "a1_data is incorrect size after realloc")
    call assert_true(size(a2_data) == 15, &
         "a2_data is incorrect size after realloc")

    call assert_true(all(a1_data(1:5) == 5.0), &
         "a1_data does not compare after realloc")
    call assert_true(all(a2_data(1:5) == -5), &
         "a2_data does not compare after realloc")

    a1_data(6:10) = 10.0
    a2_data(6:10) = -10

    a2_data(11:15) = -15

!--  EXPECT_EQ(ATK_dataview_get_total_bytes(a1), sizeof(float)*10)
!--  EXPECT_EQ(ATK_dataview_get_total_bytes(a2), sizeof(int)*15)

    call ds%print()
    call ds%delete()

  end subroutine int_array_realloc

!------------------------------------------------------------------------------

  subroutine simple_opaque()
    type(datastore) ds
    type(datagroup) root
    type(dataview) opq_view
    integer(C_INT), target :: src_data
    integer(C_INT), pointer :: out_data
    type(C_PTR) src_ptr, opq_ptr

    call set_case_name("simple_opaque")

    ! create our main data store
    ds = datastore_new()

    ! get access to our root data Group
    root = ds%get_root()

    src_data = 42
   
    src_ptr = c_loc(src_data)

    opq_view = root%create_view_external("my_opaque", src_ptr)

    ! External pointers are held in the view, not buffer
    call assert_true(ds%get_num_buffers() == 0)

    call assert_true(opq_view%is_external(), "opq_view%is_external()")
    call assert_true(opq_view%is_applied() .eqv. .false., "opq_view%is_applied()")
    call assert_true(opq_view%is_opaque(), "opq_view%is_opaque()")

    opq_ptr = opq_view%get_void_ptr()
    call c_f_pointer(opq_ptr, out_data)

    call assert_true(c_associated(opq_ptr, src_ptr), "c_associated(opq_ptr,src_ptr)")
    call assert_equals(out_data, 42, "out_data, 42")

    call ds%print()
    call ds%delete()
!--  free(src_data)
  end subroutine simple_opaque

!----------------------------------------------------------------------
end module sidre_view
!----------------------------------------------------------------------

program fortran_test
  use fruit
  use sidre_view
  implicit none
  logical ok

  call init_fruit

  call create_views
  call int_buffer_from_view
  call int_buffer_from_view_conduit_value
  call int_array_multi_view
  call init_int_array_multi_view
  call int_array_depth_view
  call int_array_view_attach_buffer
  call int_array_multi_view_resize
  call int_array_realloc
  call simple_opaque

  call fruit_summary
  call fruit_finalize

  call is_all_successful(ok)
  if (.not. ok) then
     call exit(1)
  endif
end program fortran_test
