sap.ui.define([
        'jquery.sap.global',
        'sap/ui/core/mvc/Controller',
        'sap/ui/model/json/JSONModel',
        "sap/ui/model/resource/ResourceModel",
        'gonzalo123/model/io'
    ],

    function (jQuery, Controller, JSONModel, ResourceModel, io) {
        "use strict";

        io.connect("//192.168.1.104:3000/");

        return Controller.extend("gonzalo123.controller.Controller", {
            model: new JSONModel({
                light: {
                    status: false,
                    level: undefined
                },
                potentiometer: undefined,
                temperature: undefined
            }),

            onInit: function () {
                var model = this.model;
                io.on('mqtt', function (data) {
                    switch (data.topic) {
                        case 'sensors/arduino/temperature/room1':
                            model.setProperty("/temperature", data.payload);
                            break;
                        case 'sensors/arduino/light/room1':
                            model.setProperty("/light/level", data.payload);
                            break;
                        case 'sensors/nodemcu/potentiometer/room1':
                            model.setProperty("/potentiometer", data.payload);
                            break;
                        case 'sensors/arduino/light/status':
                            model.setProperty("/light/status", data.payload == "1");
                            break;
                    }
                });

                this.getView().setModel(this.model);

                var i18nModel = new ResourceModel({
                    bundleName: "gonzalo123.i18n.i18n"
                });
                this.getView().setModel(i18nModel, "i18n");
            },

            onStatusChange: function (oEvent) {
                var status = this.getView().getModel().oData.light.status
                console.log("onStatusChange", status);
                io.emit('mqtt', {
                    topic: 'sensors/arduino/light/change',
                    payload: (status ? "1" : "0")
                });
            },

            onColorChange: function (oEvent) {
                var color = oEvent.getSource().data("color");
                var value = this.getView().getModel().oData.light[color];
                console.log(color, value);
                io.emit('mqtt', {
                    topic: 'sensors/arduino/light/' + color + '/change',
                    payload: value
                });
            }
        });
    }
);