package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionImpl;

public class sendFrame {
    private String messageId;
    private String dest;
    private String msg;
    public sendFrame(String msg,int msgId, String dest){
        this.msg=msg;
        this.messageId=""+msgId;
        this.dest=dest;
    }
    public String procces(){//we get hers ony if we succeded. sending without subId, will put the rigth one in the send function of connection
       return "MESSAGE\nsubscription:\nmessage-id:"+messageId+"\ndestination:"+dest+"\n"+msg+"\u0000";
    }
}