using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Web;


namespace Client
{
    internal class BinaryConverter
    {
        private const int BYTE_SIZE = 8;

        /**
         * Converts a string into a byte array by casting each character to a byte
         * @param str the string to convert
         * @return a byte array representation of the string
         */
        public static byte[] strToBin(string str)
        {
            byte[] result = new byte[str.Length];
            for (int i = 0; i < str.Length; i++)
                result[i] = (byte)str[i];
            return result;
        }

        /**
         * Converts an integer into a byte array of a specific size using bitwise operations
         * @param num the integer to convert
         * @param bytes the number of bytes for the resulting array
         * @return a byte array representation of the integer
         */
        public static byte[] intToBin(int num, int bytes)
        {
            byte[] result = new byte[bytes];
            for (int i = bytes - 1; i >= 0; i--)
            {
                result[i] = (byte)(num & 0xFF);
                num >>= BYTE_SIZE;
            }
            return result;
        }

        /**
         * Converts a byte array back into a string by casting each byte to a character
         * @param bin the byte array to convert
         * @return a string representation of the byte array
         */
        public static string binToStr(byte[] bin)
        {
            char[] result = new char[bin.Length];
            for (int i = 0; i < bin.Length; i++)
                result[i] = (char)bin[i];
            return new string(result);
        }

        /**
         * Converts a byte array into an integer using bitwise operations
         *  Throws an exception if the byte array is null or empty
         * @param bin the byte array to convert
         * @return the integer representation of the byte array
         */
        public static int binToInt(byte[] bin)
        {
            if (bin == null || bin.Length == 0)
                throw new ArgumentException("Invalid binary data");
            int result = 0;
            for (int i = 0; i < bin.Length; i++)
            {
                result = (result << BYTE_SIZE) | bin[i];
            }
            return result;
        }
    }
}