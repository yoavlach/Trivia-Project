using System;
using System.Collections.Generic;
using System.Linq;
using System.Printing.IndexedProperties;
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

    public struct RoomData
    {
        public int id;
        public string name;
        public uint maxPlayers;
        public uint numOfQuestionsInGame;
        public uint timePerQuestion;
        public int status;
    }

    public enum ROOM_DATA_INDEXES { ID = 0, MAX_PLAYERS = 1, NAME = 2, NUM_OF_QUESTIONS_IN_GAME = 3, STATUS = 4, TIME_PER_QUESTION = 5 };

    /// <summary>
    /// Interaction logic for RoomLobbyWindow.xaml
    /// </summary>
    public partial class RoomLobbyWindow : Window
    {
        private RoomData m_roomData;
        public RoomLobbyWindow(int roomID)
        {
            InitializeComponent();
            getRoomDataFromID(roomID);
            refreshPlayersInRoom();
        }

        private void refreshPlayersInRoom()
        {
            playersPanel.Children.Clear();
            byte[] resp = Communicator.sendAndReceive(JsonRequestPacketSerializer.serializeRequest(new GetPlayersInRoomRequest { roomId = m_roomData.id }));
            string formatted = "";
            GetPlayersInRoomResponse deserializedResponse;
            switch(Helper.extractMessageCode(resp))
            {
                case (int)MESSAGE_CODES.GET_PLAYERS_IN_ROOM:
                    deserializedResponse = JsonResponsePacketDeserializer.deserializeGetPlayersResponse(resp);
                    if (deserializedResponse.players.Length == 0)
                        formatted = "No players in room";
                    else
                    {
                        for (int i = 0; i < deserializedResponse.players.Length; i++)
                        {
                            formatted = deserializedResponse.players[i];
                            if (i == 0)
                                formatted += "- Admin";
                            playersPanel.Children.Add(new TextBlock
                            {
                                Text = formatted,
                                Margin = new Thickness(5)
                            });
                        }
                    }
                    break;
                case (int)STATUSES.FAILURE:
                    MessageBox.Show("Unable to get players in room. Please try again");
                    break;
            }
        }

        private void refreshButtonPressed(object sender, RoutedEventArgs e)
        {
            refreshPlayersInRoom();
        }

        private void getRoomDataFromID(int roomID)
        {
            byte[] response;
            GetRoomsResponse deserializedResponse;
            response = Communicator.sendAndReceive(JsonRequestPacketSerializer.serializeRequest(MESSAGE_CODES.GET_ROOMS));
            try
            {
                switch (Helper.extractMessageCode(response))
                {
                    case (int)MESSAGE_CODES.GET_ROOMS:
                        deserializedResponse = JsonResponsePacketDeserializer.deserializeGetRoomsResponse(response);
                        switch(deserializedResponse.status)
                        {
                            case (int)STATUSES.SUCCESS:
                                for (int i = 0; i < deserializedResponse.rooms.Length; i++)
                                {
                                    if (deserializedResponse.rooms[i].id == roomID)
                                        m_roomData = deserializedResponse.rooms[i];
                                }
                                roomNameLabel.Content = m_roomData.name;
                                break;
                            case (int)STATUSES.FAILURE:
                                MessageBox.Show("Unable to get rooms");
                                Close();
                                break;
                        }
                        break;
                    case (int)STATUSES.FAILURE:
                        MessageBox.Show(JsonResponsePacketDeserializer.deserializeErrorResponse(response).message);
                        Close();
                        break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected Error: " + ex.Message);
                return;
            }
        }
    }
}
