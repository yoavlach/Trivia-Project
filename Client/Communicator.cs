using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Client
{
    public static class Communicator
    {
        public static TcpClient m_client;
        public static NetworkStream m_stream;
        public static bool m_socketIntialized;

        /**
         * Sends a request to the server through the network stream and waits for a response
         *  Displays an error message box and returns null if an exception occurs
         * @param request the byte array containing the request data to send
         * @return a byte array containing the response from the server
         */
        public static byte[] sendAndReceive(byte[] request)
        {
            try
            {
                Helper.sendData(m_stream, request);
                return Helper.getMessage(m_stream);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected Error: " + ex.Message);
                return null;
            }
        }
    }
}