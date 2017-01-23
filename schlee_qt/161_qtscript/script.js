function MyClass(label)
{
    this.label = label
}

MyClass.prototype.updateText = function(str)
{
    this.label.text = str;
}

function updateText(str)
{
    wgt.label1.text = str;
}

wgt.windowsTitle = "Signal And Slots";

wgt.lineedit.text = "Test";
wgt.lineedit.textChanged.connect(updateText);

var myObject = new MyClass(wgt.label2);
wgt.lineedit.textChanged.connect(myObject, "updateText");

wgt.lineedit.textChanged.connect(wgt.label3.setText);
