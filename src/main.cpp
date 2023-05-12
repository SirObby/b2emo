#include "json.hpp"
#include <dpp/dpp.h>

int main()
{
    
    std::cout << "begin read";
    json configdocument;
    std::ifstream configfile("./config.json");
    configfile >> configdocument;
    configfile.close();

    std::cout << "set token";
    dpp::cluster bot(configdocument["token"], dpp::i_default_intents |  dpp::i_message_content, 1);


    bot.on_log(dpp::utility::cout_logger());

    /* The event is fired when someone issues your commands */
    bot.on_slashcommand([&bot](const dpp::slashcommand_t &event)
                        {
	        /* Check which command they ran */
	        if (event.command.get_command_name() == "blep") {
	            /* Fetch a parameter value from the command parameters */
	            std::string animal = std::get<std::string>(event.get_parameter("animal"));
	            /* Reply to the command. There is an overloaded version of this
	            * call that accepts a dpp::message so you can send embeds.
	            */
	            event.reply(std::string("Blep! You chose") + animal);
	        } });
    bot.on_ready([&bot](const dpp::ready_t &event)
                 {
                    std::cout << "onready";
	        if (dpp::run_once<struct register_bot_commands>()) {
                std::cout << "register";

	            /* Create a new global command on ready event */
	            dpp::slashcommand newcommand("blep", "Send a random adorable animal photo", bot.me.id);
	            newcommand.add_option(
	                    dpp::command_option(dpp::co_string, "animal", "The type of animal", true).
	                        add_choice(dpp::command_option_choice("Dog", std::string("animal_dog"))).
	                        add_choice(dpp::command_option_choice("Cat", std::string("animal_cat"))).
	                        add_choice(dpp::command_option_choice("Penguin", std::string("animal_penguin")
	                    )
	                )
	            );
	 
	            /* Register the command */
	            bot.global_command_create(newcommand);

                dpp::slashcommand infocommand("info", "Get bot info", bot.me.id);
	 
	            /* Register the command */
	            bot.global_command_create(infocommand);
	        } });

    std::cout << "start";

    bot.start(dpp::st_wait);

    return 0;
}