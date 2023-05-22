package bgu.spl.net.impl.stomp;

import java.sql.Connection;
import bgu.spl.net.srv.ConnectionImpl;

import bgu.spl.net.srv.ConnectionImpl;

public class connectFrame {
    private String receipt_id=null;
    private String accept_version=null;
    private String login=null;
    private String passcode=null;
    private int headerCount=0;
    private String[]split;
    private int connectionId;
    private ConnectionImpl conn;
    public connectFrame(String[]split,int connectionId,ConnectionImpl con){
        this.split=split;
        this.connectionId=connectionId;
        this.conn=con;
    }

    public String procces(){
        for (String line : split) {//this for finds all the headers
            String []thisline=line.split(":"); 
            if(thisline[0].equals("receipt")){
                this.receipt_id=thisline[1];
            }
            else if(thisline[0].equals("accept-version")){
                this.accept_version="1.2";
                headerCount++;
            }
            else if(thisline[0].equals("host")){headerCount++;}
            else if(thisline[0].equals("login")){if(thisline.length>1){this.login=thisline[1];headerCount++;}}
            else if(thisline[0].equals("passcode")){if(thisline.length>1){this.passcode=thisline[1];headerCount++;}}
        }
        if(headerCount!=4){//missing headers
            if(receipt_id!=null){return "ERROR\nreceipt-id:"+receipt_id+"\nmessage:one of the headers is missing\u0000";}
            else{return "ERROR\nmessage:one of the headers is missing\u0000";} 
        }
        else{
            String ans=DataBase.getInstance().login(connectionId,login, passcode);//login to data base
            if(ans.equals("connected")){
                if(receipt_id!=null){
                    conn.send(connectionId,"RECEIPT\nreceipt-id:"+receipt_id);//we need to send an reciept messege as well
                }
                return "CONNECTED\nversion:1.2\u0000"; 
            }
            else{//ans is an informative messege to why he could not connect
                if(receipt_id!=null){return "ERROR\nreceipt-id:"+receipt_id+"\nmessage:"+ans+"\u0000"; }
                return "ERROR\nmessage:"+ans+"\u0000"; 
            }
        }
    }
}
