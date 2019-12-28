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
          <Message positive>
            <Message.Header>Welcome and take it easy, we're still pre-alpha</Message.Header>
            <p>We have plenty of bugs from an alarming number of hacks but the frontend can almost be<br />considered functional and the webasm component usually works</p>
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
