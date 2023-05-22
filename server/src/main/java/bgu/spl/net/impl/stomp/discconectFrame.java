package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Connections;

public class discconectFrame {
    private String receipt_id=null;    
    private int headerCount=0;
    private String[]split;
    private int connectionId;
    public discconectFrame(String[]split,int connectionId){
        this.split=split;
        this.connectionId=connectionId;
    }
    public String procces(){
        for (String line : split) {//finding the headers
            String []thisline=line.split(":"); 
            if(thisline[0].equals("receipt")){
                this.receipt_id=thisline[1];
                headerCount++;
            }
        }
        if(headerCount!=1){
            return "ERROR\nreceipt-id:"+receipt_id+"\nmessage:one of the headers is missing\u0000"; 
        }
        else{
            boolean succes= DataBase.getInstance().logout(connectionId);
            if(succes){
                //recite
                return "RECEIPT\nreceipt-id:"+receipt_id+"\u0000";
            }
            else{
                //error
                return "ERROR\nreceipt-id:"+receipt_id+"\ncannot logout\u0000";//not suppost to happen
            }
        }
    } 
}
