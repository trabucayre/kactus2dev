//-----------------------------------------------------------------------------
// File: tst_VerilogGenerator.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 28.7.2014
//
// Description:
// Unit test for class VerilogGenerator.
//-----------------------------------------------------------------------------

#include <QtTest>

#include <IPXACTmodels/component.h>
#include <IPXACTmodels/ComponentInstance.h>
#include <IPXACTmodels/businterface.h>
#include <IPXACTmodels/design.h>
#include <IPXACTmodels/model.h>
#include <IPXACTmodels/modelparameter.h>
#include <IPXACTmodels/port.h>
#include <IPXACTmodels/vlnv.h>

#include <Plugins/VerilogGenerator/VerilogGenerator.h>
#include "LibraryMock.h"

class tst_VerilogGenerator : public QObject
{
    Q_OBJECT

public:
    tst_VerilogGenerator();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test cases:
    void testTopLevelComponent();
    void testConsecutiveParseCalls();
    void testFileHeaderIsPrinted();
    void testHierarchicalConnections();
    void testSlicedHierarchicalConnection();

private:

    void addPort( QString const& portName, int portSize, General::Direction direction, 
        QSharedPointer<Component> component);

    void addModelParameter( QString const& name, QString const& value );

    void runGenerator();

    void createHierarchicalConnection(QString const& topInterfaceRef, QString const& instanceInterfaceRef);

    void createTestInstanceToLibraryAndDesign();

    void mapPortToInterface(QString const& portName, QString const& logicalName, 
        QString const& interfaceName, QSharedPointer<Component> component);

    void mapPortToInterface(QString const& portName, int left, int right, 
        QString const& logicalName, QString const& interfaceName, QSharedPointer<Component> component);
    
    void addInterfaceToComponent(QString const& interfaceName, QSharedPointer<Component> component);

    void verifyOutputContains(QString const& expectedOutput);

    void compareOutputTo(QString const& expectedOutput);

    void readOutputFile();

    //! The top level component for which the generator is run.
    QSharedPointer<Component> topComponent_;
    
    //! The design for which the generator is run.
    QSharedPointer<Design> design_;

    //! The generator output as a string.
    QString output_;
    
    //! The time of the generator run.
    QDateTime generationTime_;

    //! The test mock for library interface.
    LibraryMock library_;
};

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::tst_VerilogGenerator()
//-----------------------------------------------------------------------------
tst_VerilogGenerator::tst_VerilogGenerator(): topComponent_(), design_(), output_(), library_(this)
{

}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::initTestCase()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::initTestCase()
{

}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::cleanupTestCase()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::cleanupTestCase()
{

}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::init()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::init()
{
    VLNV vlnv(VLNV::COMPONENT, "Test", "TestLibrary", "TestComponent", "1.0");
    topComponent_ = QSharedPointer<Component>(new Component(vlnv));

    VLNV designVlnv(VLNV::DESIGN, "Test", "TestLibrary", "TestDesign", "1.0");
    design_ = QSharedPointer<Design>(new Design(designVlnv));

    library_.clear();

    QVERIFY(!QFile::exists("./generatorOutput.v"));
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::cleanup()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::cleanup()
{
    topComponent_.clear();

    output_.clear();

    QFile::remove("./generatorOutput.v");
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::testTopLevelComponent()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::testTopLevelComponent()
{
    addPort("clk", 1, General::IN, topComponent_);
    addPort("rst_n", 1, General::IN, topComponent_);
    addPort("dataOut", 8, General::OUT, topComponent_);
    addPort("dataIn", 8, General::IN, topComponent_);
    addModelParameter("dataWidth", "8");
    addModelParameter("freq", "100000");

    runGenerator();

    verifyOutputContains(QString("module TestComponent(clk, dataIn, rst_n, dataOut);\n"
        "    parameter dataWidth = 8;\n"
        "    parameter freq = 100000;\n"
        "    // These ports are not in any interface\n"         
        "    input clk;\n"
        "    input [7:0] dataIn;\n"
        "    input rst_n;\n"
        "    output [7:0] dataOut;\n"
        "\n"
        "endmodule\n"
        ));
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::addPort()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::addPort(QString const& portName, int portSize, 
    General::Direction direction, QSharedPointer<Component> component)
{
    QSharedPointer<Port> port = QSharedPointer<Port>(new Port(portName, direction, portSize - 1, 0, "", true));
    component->addPort(port);
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::addModelParameter()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::addModelParameter( QString const& name, QString const& value )
{
    QSharedPointer<ModelParameter> parameter = QSharedPointer<ModelParameter>(new ModelParameter());
    parameter->setName(name);
    parameter->setValue(value);
    topComponent_->getModel()->addModelParameter(parameter);
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::runGenerator()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::runGenerator()
{
    VerilogGenerator generator(&library_);

    generator.parse(topComponent_, design_);

    generationTime_ =  QDateTime::currentDateTime();

    generator.generate("./generatorOutput.v");
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::testFileHeaderIsPrinted()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::testFileHeaderIsPrinted()
{
    topComponent_->setDescription("Component description\nspanning multiple\nlines.");
        
    runGenerator();
    
    compareOutputTo(QString(
        "//-----------------------------------------------------------------------------\n"
        "// File          : generatorOutput.v\n"
        "// Creation date : " + generationTime_.date().toString(Qt::LocalDate) + "\n"
        "// Creation time : " + generationTime_.time().toString(Qt::LocalDate) + "\n"
        "// Description   : Component description\n"
        "//                 spanning multiple\n"
        "//                 lines.\n"
        "// Created by    : user\n"
        "// This file was generated with Kactus2 verilog generator\n"
        "// based on IP-XACT component Test:TestLibrary:TestComponent:1.0\n"
        "// whose XML file is C:/Test/TestLibrary/TestComponent/1.0/TestComponent.1.0.xml\n"
        "//-----------------------------------------------------------------------------\n"
        "\n"
        "module TestComponent();\n"
        "\n"
        "endmodule\n"));
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::testConsecutiveParseCalls()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::testConsecutiveParseCalls()
{
    VLNV secondVlnv(VLNV::COMPONENT, "Test", "TestLibrary", "Override", "1.0");
    QSharedPointer<Component> secondComponent = QSharedPointer<Component>(new Component());
    secondComponent->setVlnv(secondVlnv);

    VerilogGenerator generator(&library_);

    generator.parse(topComponent_);
    generator.parse(secondComponent);

    generator.generate("./generatorOutput.v");
    
    verifyOutputContains(QString("module Override();\n"
        "\n"
        "endmodule\n"));
    QVERIFY(!output_.contains("module TestComponent"));
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::testHierarchicalConnections()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::testHierarchicalConnections()
{
    addPort("top_clk", 1, General::IN, topComponent_);
    addPort("data_to_instance", 8, General::IN, topComponent_);
    addPort("enable_to_instance", 1, General::IN, topComponent_);
    addPort("full_from_instance", 1, General::OUT, topComponent_);

    addInterfaceToComponent("clk_if", topComponent_);
    addInterfaceToComponent("data_bus", topComponent_);
    
    mapPortToInterface("top_clk", "CLK", "clk_if", topComponent_);
    mapPortToInterface("data_to_instance", "DATA", "data_bus", topComponent_);
    mapPortToInterface("enable_to_instance", "ENABLE", "data_bus", topComponent_);
    mapPortToInterface("full_from_instance", "FULL", "data_bus", topComponent_);

    createHierarchicalConnection("clk_if", "clk");
    createHierarchicalConnection("data_bus", "data");

    createTestInstanceToLibraryAndDesign();

    runGenerator();

    verifyOutputContains(QString(
        "module TestComponent(top_clk, data_to_instance, enable_to_instance, full_from_instance);\n"
        "    // Interface: clk_if\n"
        "    input top_clk;\n"
        "\n"
        "    // Interface: data_bus\n"     
        "    input [7:0] data_to_instance;\n"
        "    input enable_to_instance;\n"
        "    output full_from_instance;\n"
        "\n"
        "TestInstance instance1(\n"
        "    .clk(top_clk),\n"
        "    .data_in(data_to_instance),\n"
        "    .enable(enable_to_instance),\n"
        "    .full(full_from_instance),\n"
        "    .data_out( ));\n"
        "endmodule\n"));
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::testHierarchicalConnections()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::testSlicedHierarchicalConnection()
{    
    addInterfaceToComponent("data_bus", topComponent_);
    addPort("enable_to_instance", 2, General::IN, topComponent_);
    addPort("data_to_instance", 16, General::IN, topComponent_);
    addPort("full_from_instance", 3, General::OUT, topComponent_);   
    mapPortToInterface("enable_to_instance", 1, 1, "ENABLE", "data_bus", topComponent_);
    mapPortToInterface("data_to_instance", 7, 0, "DATA", "data_bus", topComponent_);
    mapPortToInterface("full_from_instance", 1, 1, "FULL", "data_bus", topComponent_);

    createHierarchicalConnection("data_bus", "data");
    createHierarchicalConnection("clk_if", "clk");

    createTestInstanceToLibraryAndDesign();

    runGenerator();

    verifyOutputContains(QString(
        "TestInstance instance1(\n"
        "    .clk( ),\n"
        "    .data_in(data_to_instance[7:0]),\n"
        "    .enable(enable_to_instance[1]),\n"
        "    .full(full_from_instance[1]),\n"
        "    .data_out( ));"));
}
 
//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::addInterfaceToComponent()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::addInterfaceToComponent(QString const& interfaceName,
    QSharedPointer<Component> component)
{
    QSharedPointer<BusInterface> busInterface(new BusInterface());
    busInterface->setName(interfaceName);
    component->addBusInterface(busInterface);
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::addPortMap()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::mapPortToInterface(QString const& portName, QString const& logicalName, 
    QString const& interfaceName, QSharedPointer<Component> component)
{
    QSharedPointer<General::PortMap> portMap(new General::PortMap());
    portMap->logicalPort_ = logicalName;
    portMap->physicalPort_ = portName;
    portMap->physicalVector_->setLeft(component->getPortLeftBound(portName));
    portMap->physicalVector_->setRight(component->getPortRightBound(portName));
    QList<QSharedPointer<General::PortMap> > portMaps = component->getBusInterface(interfaceName)->getPortMaps();
    portMaps.append(portMap);
    component->getBusInterface(interfaceName)->setPortMaps(portMaps);
}
//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::mapPortToInterface()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::mapPortToInterface(QString const& portName, int left, int right, QString const& logicalName, 
    QString const& interfaceName, QSharedPointer<Component> component)
{
    QSharedPointer<General::PortMap> portMap(new General::PortMap());
    portMap->logicalPort_ = logicalName;
    portMap->physicalPort_ = portName;
    portMap->physicalVector_->setLeft(left);
    portMap->physicalVector_->setRight(right);
    QList<QSharedPointer<General::PortMap> > portMaps = component->getBusInterface(interfaceName)->getPortMaps();
    portMaps.append(portMap);
    component->getBusInterface(interfaceName)->setPortMaps(portMaps);
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::createInstance()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::createTestInstanceToLibraryAndDesign()
{
    VLNV instanceVlnv(VLNV::COMPONENT, "Test", "TestLibrary", "TestInstance", "1.0");

    QSharedPointer<Component> instanceComponent(new Component(instanceVlnv));
    addPort("clk", 1, General::IN, instanceComponent);
    addPort("data_in", 8, General::IN, instanceComponent);
    addPort("data_out", 8, General::OUT, instanceComponent);
    addPort("enable", 1, General::IN, instanceComponent);
    addPort("full", 1, General::OUT, instanceComponent);

    addInterfaceToComponent("clk", instanceComponent);

    mapPortToInterface("clk", "CLK", "clk", instanceComponent);

    addInterfaceToComponent("data", instanceComponent);

    mapPortToInterface("data_in", "DATA", "data", instanceComponent);
    mapPortToInterface("enable", "ENABLE", "data", instanceComponent);
    mapPortToInterface("full", "FULL", "data", instanceComponent);

    library_.addComponent(instanceComponent);

    ComponentInstance instance("instance1", "", "", instanceVlnv, QPointF(), "");

    QList<ComponentInstance> componentInstances = design_->getComponentInstances();
    componentInstances.append(instance);
    design_->setComponentInstances(componentInstances);
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::createHierarchicalConnection()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::createHierarchicalConnection(QString const& topInterfaceRef, QString const& instanceInterfaceRef)
{
    HierConnection clkConnection(topInterfaceRef, Interface("instance1", instanceInterfaceRef));    
    QList<HierConnection> hierConnections = design_->getHierarchicalConnections();
    hierConnections.append(clkConnection);
    design_->setHierarchicalConnections(hierConnections);
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::verifyOutputContains()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::verifyOutputContains(QString const& expectedOutput)
{
    readOutputFile();

    if (!output_.contains(expectedOutput))
    {
        QStringList outputLines = output_.split("\n");
        QStringList expectedLines = expectedOutput.split("\n");

        QVERIFY(outputLines.count() >= expectedLines.count());

        int lineOffset = outputLines.indexOf(expectedLines.first());

        if (lineOffset == -1)
        {
            compareOutputTo(expectedOutput);
        }
        else
        {
            int lineCount = expectedLines.count();
            for (int i = 0; i < lineCount; i++)
            {
                QCOMPARE(outputLines.at(i + lineOffset), expectedLines.at(i));
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::compareLineByLine()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::compareOutputTo(QString const& expectedOutput)
{
    readOutputFile();

    QCOMPARE(output_, expectedOutput);
}

//-----------------------------------------------------------------------------
// Function: tst_VerilogGenerator::readOutputFile()
//-----------------------------------------------------------------------------
void tst_VerilogGenerator::readOutputFile()
{
    QFile outputFile("./generatorOutput.v");

    QVERIFY(outputFile.open(QIODevice::ReadOnly));

    output_ = outputFile.readAll();
}


QTEST_APPLESS_MAIN(tst_VerilogGenerator)

#include "tst_VerilogGenerator.moc"