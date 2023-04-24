(module
  (type $t0 (func (result i32)))
  (type $t1 (func (param i32 i32)))
  (type $t2 (func (param i32)))
  (type $t3 (func))
  (import "arduino" "print" (func $arduino.print (type $t1)))
  (import "arduino" "getGreeting" (func $arduino.getGreeting (type $t1)))
  (import "arduino" "getx" (func $arduino.getx (type $t0)))
  (import "arduino" "gety" (func $arduino.gety (type $t0)))
  (import "arduino" "getz" (func $arduino.getz (type $t0)))
  (import "arduino" "gettemp" (func $arduino.gettemp (type $t0)))
  (import "arduino" "printn" (func $arduino.printn (type $t2)))
  (func $_start (type $t3)
    (local $l0 i32) (local $l1 i32) (local $l2 i32) (local $l3 i32) (local $l4 f64)
    global.get $g0
    i32.const -64
    i32.add
    local.tee $l0
    global.set $g0
    i32.const 1299
    i32.const 1
    call $arduino.print
    i32.const 1024
    i32.const 52
    call $arduino.print
    i32.const 1299
    i32.const 1
    call $arduino.print
    i32.const 1077
    i32.const 8
    call $arduino.print
    local.get $l0
    i32.const 64
    call $arduino.getGreeting
    local.get $l0
    i32.const 1
    i32.sub
    local.set $l1
    loop $L0
      local.get $l1
      i32.const 1
      i32.add
      local.tee $l1
      i32.load8_u
      br_if $L0
    end
    local.get $l0
    local.get $l1
    local.get $l0
    i32.sub
    call $arduino.print
    i32.const 1299
    i32.const 1
    call $arduino.print
    i32.const 1086
    i32.const 1
    call $arduino.print
    local.get $l0
    i32.const -64
    i32.sub
    global.set $g0
    loop $L1
      call $arduino.getx
      local.set $l0
      call $arduino.gety
      local.set $l1
      call $arduino.getz
      local.set $l2
      call $arduino.gettemp
      local.set $l3
      i32.const 1299
      i32.const 1
      call $arduino.print
      i32.const 1086
      i32.const 1
      call $arduino.print
      i32.const 1299
      i32.const 1
      call $arduino.print
      i32.const 1088
      i32.const 85
      call $arduino.print
      i32.const 1299
      i32.const 1
      call $arduino.print
      i32.const 1174
      i32.const 9
      call $arduino.print
      local.get $l0
      call $arduino.printn
      i32.const 1184
      i32.const 6
      call $arduino.print
      i32.const 1191
      i32.const 9
      call $arduino.print
      local.get $l1
      call $arduino.printn
      i32.const 1184
      i32.const 6
      call $arduino.print
      i32.const 1201
      i32.const 9
      call $arduino.print
      local.get $l2
      call $arduino.printn
      i32.const 1184
      i32.const 6
      call $arduino.print
      i32.const 1211
      i32.const 15
      call $arduino.print
      local.get $l3
      call $arduino.printn
      i32.const 1227
      i32.const 3
      call $arduino.print
      i32.const 1299
      i32.const 1
      call $arduino.print
      i32.const 1086
      i32.const 1
      call $arduino.print
      i32.const 1299
      i32.const 1
      call $arduino.print
      i32.const 1231
      i32.const 55
      call $arduino.print
      i32.const 1299
      i32.const 1
      call $arduino.print
      i32.const 1086
      i32.const 1
      call $arduino.print
      i32.const 1299
      i32.const 1
      call $arduino.print
      i32.const 1174
      i32.const 9
      call $arduino.print
      block $B2 (result i32)
        local.get $l0
        f64.convert_i32_s
        f64.const 0x1.a3f28fd4f4b98p+1 (;=3.28084;)
        f64.mul
        local.tee $l4
        f64.abs
        f64.const 0x1p+31 (;=2.14748e+09;)
        f64.lt
        if $I3
          local.get $l4
          i32.trunc_f64_s
          br $B2
        end
        i32.const -2147483648
      end
      call $arduino.printn
      i32.const 1287
      i32.const 7
      call $arduino.print
      i32.const 1191
      i32.const 9
      call $arduino.print
      block $B4 (result i32)
        local.get $l1
        f64.convert_i32_s
        f64.const 0x1.a3f28fd4f4b98p+1 (;=3.28084;)
        f64.mul
        local.tee $l4
        f64.abs
        f64.const 0x1p+31 (;=2.14748e+09;)
        f64.lt
        if $I5
          local.get $l4
          i32.trunc_f64_s
          br $B4
        end
        i32.const -2147483648
      end
      call $arduino.printn
      i32.const 1287
      i32.const 7
      call $arduino.print
      i32.const 1201
      i32.const 9
      call $arduino.print
      block $B6 (result i32)
        local.get $l2
        f64.convert_i32_s
        f64.const 0x1.a3f28fd4f4b98p+1 (;=3.28084;)
        f64.mul
        local.tee $l4
        f64.abs
        f64.const 0x1p+31 (;=2.14748e+09;)
        f64.lt
        if $I7
          local.get $l4
          i32.trunc_f64_s
          br $B6
        end
        i32.const -2147483648
      end
      call $arduino.printn
      i32.const 1287
      i32.const 7
      call $arduino.print
      i32.const 1211
      i32.const 15
      call $arduino.print
      block $B8 (result i32)
        local.get $l3
        f64.convert_i32_s
        f64.const 0x1.ccccccccccccdp+0 (;=1.8;)
        f64.mul
        f64.const 0x1p+5 (;=32;)
        f64.add
        local.tee $l4
        f64.abs
        f64.const 0x1p+31 (;=2.14748e+09;)
        f64.lt
        if $I9
          local.get $l4
          i32.trunc_f64_s
          br $B8
        end
        i32.const -2147483648
      end
      call $arduino.printn
      i32.const 1295
      i32.const 3
      call $arduino.print
      br $L1
    end
    unreachable)
  (memory $memory 1)
  (global $g0 (mut i32) (i32.const 3360))
  (export "memory" (memory $memory))
  (export "_start" (func $_start))
  (data $d0 (i32.const 1024) "METRIC UNIT TO EMPERIAL CONVERSION running on WASM3!\00Status: \00 \00-------------------------------------------------------------------------------------\00 Acc_x = \00m/s^2 \00 Acc_y = \00 Acc_z = \00 Temperature = \00\c2\b0C\00                     METRIC UNIT TO EMPERIAL CONVERSION\00ft/s^2 \00\c2\b0F\00\0a"))
