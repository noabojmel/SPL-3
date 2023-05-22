package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.StompProtocol;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

    private final StompMessagingProtocol<T> protocol;//stomp protocol
    private final MessageEncoderDecoder<T> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;

    public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<T> reader, StompMessagingProtocol<T> protocol, int uniqeId,Connections connection) {//added the uniqe id the server gave to client and connections
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
        this.protocol.start(uniqeId,connection);//here the protocol need to start after opening the connection handler
    }

    @Override
    public void run() {
        try (Socket sock = this.sock) { //just for automatic closing
            int read;

            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());

            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                T nextMessage = encdec.decodeNextByte((byte) read);
                if (nextMessage != null&&!nextMessage.equals("")) {
                   /* T response =*/  protocol.process(nextMessage);//our pprocces is void so there is no response
                   // if (response != null) {
                   //     out.write(encdec.encode(response));
                    //    out.flush();
                    }
                }
            }

         catch (IOException ex) {
            ex.printStackTrace();
        }

    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    @Override
    public void send(T msg) {
        try{
        out.write(encdec.encode(msg));      //writing string to stream
        out.flush();
    } catch (IOException e) {
        e.printStackTrace();
    }
        
    }
}
