var url = "http://192.168.64.107";

function OnStart() {
    //Create a layout with objects vertically centered.
    lay = app.CreateLayout( "linear", "VCenter,FillXY" );   
    app.AddLayout( lay );
    
    btnLightON = app.CreateButton("Light on", 0.3, 0.1);
    btnLightON.SetMargins( 0, 0.05, 0, 0 );
    btnLightON.SetOnTouch(btnLightON_OnTouch);
    lay.AddChild(btnLightON);
    
    btnLightOFF = app.CreateButton("Light off", 0.3, 0.1);
    btnLightOFF.SetMargins( 0, 0.05, 0, 0 );
    btnLightOFF.SetOnTouch(btnLightOFF_OnTouch);
    lay.AddChild(btnLightOFF);
}

function btnLightON_OnTouch() {
    var path = "/light/on";
    app.HttpRequest("get", url, path, "", HandleReply);
}

function btnLightOFF_OnTouch() {
    var path = "/light/off";
    app.HttpRequest("get", url, path, "", HandleReply);
}


//Handle the servers reply.
function HandleReply( error, response )
{
     console.log(error);
     app.ShowPopup(response);
}