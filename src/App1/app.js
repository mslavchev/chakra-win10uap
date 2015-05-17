(function () {
    debugger;

    var greeter = new WindowsRuntimeComponent1.Greeter();
    
    var s = greeter.sayHello();

    var newGreeter = createGreeter();

    s += sayWorld(newGreeter);

    return s;
}
)()