import java.net.Socket;
import java.net.URL;
import java.net.URLConnection;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Client
{
    public static void main(String[] args)
    {
        int count = 0;
        while(count++ < 5)
        {
            try
            {
                URL localhost = new URL("http://127.0.0.1:9000/get");
                URLConnection urlcon = localhost.openConnection();
                BufferedReader reader = new BufferedReader(new InputStreamReader(urlcon.getInputStream()));

                int c;
                while((c = reader.read()) != -1)
                {
                    System.out.println((char) c);
                }
            }
            catch(IOException ie)
            {
                ie.printStackTrace();
            }
        }
    }
}
