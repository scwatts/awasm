import React from 'react';


import { Grid, Header, Message } from 'semantic-ui-react'


import "./App.css";


import SemanticButtonGroup from './components/SemanticButtonGroup'
import SemanticFileInput from './components/SemanticFileInput'
import SemanticStep from './components/SemanticStep'


function App() {
  return (
    <>
      <Grid centered style={{ paddingTop: '7em' }}>
        <Grid.Row>
          <Header size='huge' textAlign='center'>awasm</Header>
        </Grid.Row>
        <Grid.Row>
          <Message positive style={{ maxWidth: '40em' }}>
            <Message.Header>Welcome and take it easy</Message.Header>
            <p>
              This small webapp will take a set of gzip compressed paired reads and
              assembles them locally on your computer or mobile device using a stripped
              version of velvet compiled to webasm. This is a basic prototype and
              things may go wrong!
            </p>
          </Message>
        </Grid.Row>
        <Grid.Row>
          <SemanticStep />
        </Grid.Row>
        <Grid.Row>
          <SemanticFileInput button_label='forward' idname='seq1' />
          <SemanticFileInput button_label='reverse' idname='seq2' />
        </Grid.Row>
        <Grid.Row>
          <SemanticButtonGroup />
        </Grid.Row>
      </Grid>
    </>
  );
}

export default App;
