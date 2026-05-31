using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for MainMenu.xaml
    /// </summary>
    public partial class MainMenu : Window
    {
        /**
         * Initializes the main menu window and its components
         */
        public MainMenu()
        {
            InitializeComponent();
        }

        /**
         * Handles the action when the create room button is clicked by opening the create room window
         * @param sender the object that triggered the event
         * @param e the event arguments
         */
        public void createRoomButtonPressed(object sender, RoutedEventArgs e)
        {
            CreateRoomWindow createRoomWindow = new CreateRoomWindow();
            createRoomWindow.Show();
        }

        /**
         * Handles the action when the exit button is clicked by sending a logout request to the server
         * @param sender the object that triggered the event
         * @param e the event arguments
         */
        public void exitButtonPressed(object sender, RoutedEventArgs e)
        {
            Helper.sendData(Communicator.m_stream, JsonRequestPacketSerializer.serializeRequest(MESSAGE_CODES.LOG_OUT));
            Application.Current.Shutdown();
        }

        /**
         * Handles the action when the join room button is clicked by opening the join room window
         * @param sender the object that triggered the event
         * @param e the event arguments
         */
        private void joinRoomButtonPressed(object sender, RoutedEventArgs e)
        {
            JoinRoomWindow joinRoomWindow = new JoinRoomWindow();
            joinRoomWindow.Show();
        }

        /**
         * Handles the action when the statistics button is clicked by opening the statistics window
         * @param sender the object that triggered the event
         * @param e the event arguments
         */
        private void statisticsButtonPressed(object sender, RoutedEventArgs e)
        {
            StatisticsWindow statisticsWindow = new StatisticsWindow();
            statisticsWindow.Show();
        }
    }
}