var baseAddress = Module.findBaseAddress('qagamex86.dll');
console.log("Base address of qagamex86.dll:", baseAddress);
var shouldModifyFuncB = false;


if (baseAddress) {
    var Weapon_Knife = baseAddress.add(0x06C6E0);  // Replace YOUR_OFFSET with the appropriate address from the start of the module to your function.
    var fltAddress = baseAddress.add(0x07B184);
    console.log("Knife Length Address: " + fltAddress);
    Interceptor.attach(Weapon_Knife, {
        onEnter: function(args) {
            console.log('Function Weapon_Knife was called with:', args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);  // args[0], args[1], and so on are the function's arguments.
            var floatValue = Memory.readFloat(fltAddress);
            console.log("Knife Length float Value: " + floatValue);
            Memory.protect(fltAddress, 4, 'rwx');
            Memory.writeFloat(fltAddress, 4800.0);
            console.log("Changed in 10 times");
        },
        onLeave: function(retval) {
            console.log('Function Weapon_Knife returned:', retval);
        }
    });

 var PM_Accelerate = baseAddress.add(0x035050);  // Replace YOUR_OFFSET with the appropriate address from the start of the module to your function.
    Interceptor.attach(PM_Accelerate, {
        onEnter: function(args) {
        if (args[0].toInt32() === 0x19c7684) {
            if (args[1].toInt32() === 0x43b00001) {args[1] = ptr('0x44f00001')}
        this.print = true;
        console.log('Function PM_Accelerate was called with:', args[0], args[1], args[2], args[3]);  // args[0], args[1], and so on are the function's arguments.
        }},
        onLeave: function(retval) {
        if (this.print){
        console.log('Function PM_Accelerate returned:', retval);       
        }}
    });

   var pers = 0x6A2214
    var offset = 0x478;
    var Move = baseAddress.add(0x035FB0);
    Interceptor.attach(Move, {
        onEnter: function(args) {
            console.log('Trying to read stamina.... ');
            var target_address = baseAddress.add(pers);
            var value = Memory.readU32(target_address);
            console.log("Значение по адресу " + target_address + " : " + value);
        },
        onLeave: function(retval){
            // Here you can process the function's result if needed.
        }
    });

    
    var G_Damage = baseAddress.add(0x043A50); 
    Interceptor.attach(G_Damage, {
        onEnter: function(args) {
            if (args[4].toInt32() === 0x19c7610 ||args[4].toInt32() === 0x19c7658){args[5]=ptr(0x100);}
            else{
                args[0]=ptr(0x20597908);
                args[5]=ptr(0x0);
            }
            console.log('Function G_Damage was called with:', args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10], args[11]); 
            },
        onLeave: function(retval) {
            console.log('Function G_Damage returned:', retval);   
        }
    });
} 
